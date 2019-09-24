#include <vector>
#include <array>
#include <math.h>
#include <stdio.h>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string
#include "blob.h"
#include "food.h"
#include "simulationResults.h"
#include "animation.h"

Animation::Animation()
{
	sAppName = "Natural Selection Simulation";
}

Animation::Animation(int cellCount, simulationResults &stats, std::string vidName,
			ColourStat colourStat)
			: m_gridCount{ cellCount }, m_stats{ stats }, m_vidName{vidName},
			m_colourStat{ colourStat }
{
	sAppName = "Natural Selection Simulation";
}

bool Animation::colourStatValid()
{
	switch (m_colourStat)
	{
		case ColourStat::SIZE:
		case ColourStat::SPEED:
		case ColourStat::SENSE:
			return true;
		default:
			return false;
	}		
}

std::string Animation::getColourStatStr()
{
	std::string colourStatStr;
	switch (m_colourStat)
	{
	case ColourStat::SIZE:
		colourStatStr = "Size";
		break;
	case ColourStat::SPEED:
		colourStatStr = "Speed";
		break;
	case ColourStat::SENSE:
		colourStatStr = "Sense";
		break;
	default:
		colourStatStr = "Size";
		break;
	}
	return colourStatStr;
}

std::string Animation::getDefaultFilename()
{
	std::string mapSize{ std::to_string(m_gridCount) + "m_" };
	std::string blobCount{ std::to_string(m_dailyBlobFrames[0][0].size()) + "b_" };
	std::string foodCount{ std::to_string(m_eachFoodPositions[0].size()) + "f_" };
	std::string dayCount{ std::to_string(m_dailyBlobFrames.size()) + "d_" };
	std::string colourStatStr{ getColourStatStr() };
	std::string filename{ mapSize + blobCount + foodCount + dayCount + colourStatStr };
	return filename;
}

void Animation::scaleStats(double scaleRange)
{
	std::vector<double> maxSize, maxSpeed, maxSense;
	double maxSizeVal{ 0 }, maxSpeedVal{ 0 }, maxSenseVal{ 0 };

	for (std::array<double, 10> day : m_avgBlobStats)
	{
		maxSize.push_back(day[4]);
		maxSpeed.push_back(day[5]);
		maxSense.push_back(day[6]);
	}
	maxSizeVal = getMax(maxSize);
	maxSpeedVal = getMax(maxSpeed);
	maxSenseVal = getMax(maxSense);

	switch (m_colourStat)
	{
		case (ColourStat::SIZE):
			m_colourBarMax = maxSizeVal;
			break;
		case (ColourStat::SPEED):
			m_colourBarMax = maxSpeedVal;
			break;
		case (ColourStat::SENSE):
			m_colourBarMax = maxSenseVal;
			break;
	}

	for (int i{ 0 }; i < m_dailyBlobFrames.size(); ++i)
	{
		for (int j{ 0 }; j < m_dailyBlobFrames[i].size(); ++j)
		{
			for (int k{ 0 }; k < (m_dailyBlobFrames[i])[j].size(); ++k)
			{
				double size = m_dailyBlobFrames[i][j][k][ColourStat::SIZE];
				double speed = m_dailyBlobFrames[i][j][k][ColourStat::SPEED];
				double sense = m_dailyBlobFrames[i][j][k][ColourStat::SENSE];

				m_dailyBlobFrames[i][j][k][ColourStat::SIZE] = size * (scaleRange / maxSizeVal);
				m_dailyBlobFrames[i][j][k][ColourStat::SPEED] = speed * (scaleRange / maxSpeedVal);
				m_dailyBlobFrames[i][j][k][ColourStat::SENSE] = sense * (scaleRange / maxSenseVal);
			}
		}
	}
}

void Animation::drawTriLabel(int x2, int yZero, int textScale, TriLabel label)
{
	//Draws "Mean" by default
	int index = m_colourStat + TriLabel::MEAN;
	double val{ (m_avgBlobStats[m_day][index]) };
	index = m_colourStat + label;
	double val2{ (m_avgBlobStats[m_day][index]) };

	std::string labelStr;
	switch (label)
	{
		case TriLabel::MEAN:
			labelStr = "Mean";
			break;
		case TriLabel::MAX:
			labelStr = "Max";
			if (std::abs(val2 - val) >= (1.40*m_colourBarMax / m_gridCount))
			{
				val = val2;
				break;
			}
			else
			{
				return;
			}
		case TriLabel::MIN:
			labelStr = "Min";
			if (std::abs(val2 - val) >= (1.40*m_colourBarMax / m_gridCount))
			{
				val = val2;
				break;
			}
			else
			{
				return;
			}
	}
	double yPos = yZero + (1 - val / m_colourBarMax)*(m_gridCount*m_cellSize);
	FillTriangle(x2 + (0.25* m_cellSize), yPos, x2 + (1.25*m_cellSize), yPos - (0.5*m_cellSize), 
			x2 + (1.25*m_cellSize), yPos + (0.5*m_cellSize), olc::WHITE);
	DrawString(x2 + (1.50 * m_cellSize), yPos - (0.75*m_cellSize), labelStr, olc::WHITE, textScale);
	std::string valStr{ std::to_string(val) };
	valStr.resize(4);
	DrawString(x2 + (1.50 * m_cellSize), yPos, valStr, olc::WHITE, textScale);
}

void Animation::drawColourBar()
{
	/*Colour bar changes from red (max) to magenta (mid) to blue (min)
	Red = 228, 59, 68
	Magenta = 228, 59, 152
	Blue = 0, 149, 233 */

	int xZero = ((m_homeCount + 2 * m_blackBorder)*m_cellSize);
	int x2{ xZero + 2 * m_cellSize };
	double step_r{ 0.0 }, step_g{ 0.0 }, step_b{ 0.0 };
	int n{ 0 };
	int yZero{ (m_blackBorder + 1)*m_cellSize }; // Where the colour bar starts
	int yLength{ m_gridCount*m_cellSize };
	for (int y = yZero; y < yZero + yLength; ++y)
	{
		olc::Pixel RtMtB(228 - step_r, 59 + step_g, 68.0 + step_b);
		DrawLine(xZero, y, x2, y, RtMtB);
		++n;

		if (n < (m_gridCount*m_cellSize) / 2)
		{
			step_b += ((152.0 - 68.0) / (yLength / 2));
		}
		else if (n >= (m_gridCount*m_cellSize) / 2)
		{
			step_r += ((228.0) / (yLength / 2));
			step_g += ((149.0 - 59.0) / (yLength / 2));
			step_b += ((233.0 - 152.0) / (yLength / 2));
		}
	}

	//Write which blob stat the colour bar measures
	std::string colourStatStr{ getColourStatStr() };
	int textScale{ m_blackBorder * m_cellSize / 16 };
	DrawString((xZero - textScale * 7), (m_blackBorder*m_cellSize / 2), colourStatStr, olc::WHITE, textScale);

	//Write max colour bar value
	std::string maxValStr = std::to_string(m_colourBarMax);
	maxValStr.resize(4);
	textScale = (m_blackBorder*m_cellSize / 20);
	DrawString((xZero - textScale * 3),yZero - (textScale * 9), maxValStr, olc::WHITE, textScale);

	//Write min colour bar value
	DrawString(xZero - textScale * 3, yZero + yLength + (textScale * 2), "0.00", olc::WHITE, textScale);

	// Draw triangles and labels
	drawTriLabel(x2, yZero, textScale, MEAN);
	drawTriLabel(x2, yZero, textScale, MAX);
	drawTriLabel(x2, yZero, textScale, MIN);
}

void Animation::drawBlob(int x, int y, double s)
{
	olc::Pixel lightBTR;
	olc::Pixel midBTR;
	olc::Pixel darkBTR;
	olc::Pixel blackBTR;
	olc::Pixel white{ 255,255,255,255 };

	double t;

	if (s <= m_scaleRange / 2.0)
	{
		t = (s /m_scaleRange)*2;
		olc::Pixel temp1(44.0 + (t*202.0), 232.0 - (t*115.0), (245.0 - (t*59.0)));
		lightBTR = temp1;
		olc::Pixel temp2(t*228.0, 149.0 - (t * 90.0), 233.0 - (t * 81.0));
		midBTR = temp2;
		olc::Pixel temp3(18.0 + (t * 140), 78.0 - (t * 38.0), 137.0 - (t*24.0));
		darkBTR = temp3;
		olc::Pixel temp4(38.0 + (t * 30), 43.0 + (t*3.0), 68.0 - (t * 0.0));
		blackBTR = temp4;
	}
	if (s > m_scaleRange / 2.0)
	{
		t = (2 * s / m_scaleRange) - 1;
		olc::Pixel temp1(228.0, 59.0, (186.0 - (t*64.0)));
		lightBTR = temp1;
		olc::Pixel temp2(228.0, 59.0, 152.0 - (t * 84.0));
		midBTR = temp2;
		olc::Pixel temp3(158.0, 40.0, 113.0 - (t*60.0));
		darkBTR = temp3;
		olc::Pixel temp4(68.0, 43.0 + (t*2.0), 68.0 - (t * 30.0));
		blackBTR = temp4;
	}

	int b = m_cellSize / 8;

	//y
	FillRect(x + (2 * b), y, (4 * b), b, lightBTR);
	//y+1
	FillRect(x + (1 * b), y+(1*b), (6 * b), b, lightBTR);
	//y+2
	FillRect(x, y + (2 * b), (4 * b), b, lightBTR);
	FillRect(x + (4 * b), y + (2 * b), (4 * b), b, midBTR);
	//y+3
	FillRect(x, y + (3 * b), b, b, lightBTR);
	FillRect(x + (1 * b), y + (3 * b), b, b, midBTR);
	FillRect(x + (2 * b), y + (3 * b), b, b, white);
	FillRect(x + (3 * b), y + (3 * b), (2 * b), b, midBTR);
	FillRect(x + (5 * b), y + (3 * b), b, b, white);
	FillRect(x + (6 * b), y + (3 * b), (2 * b), b, midBTR);
	//y+4
	FillRect(x, y + (4 * b), b, b, midBTR);
	FillRect(x + (1 * b), y + (4 * b), b, b, darkBTR);
	FillRect(x + (2 * b), y + (4 * b), b, b, blackBTR);
	FillRect(x + (3 * b), y + (4 * b), (2*b), b, darkBTR);
	FillRect(x + (5 * b), y + (4 * b), b, b, blackBTR);
	FillRect(x + (6 * b), y + (4 * b), (2 * b), b, midBTR);
	//y+5
	FillRect(x, y + (5 * b), (2 * b), b, midBTR);
	FillRect(x + (2 * b), y + (5 * b),(5*b), b, darkBTR);
	FillRect(x + (7 * b), y + (5 * b), b, b, midBTR);
	//y+6
	FillRect(x + (1 * b), y + (6 * b), (6*b), b, darkBTR);
	//y+7
	FillRect(x + (2 * b), y + (7 * b), (4 * b), b, darkBTR);
}

void Animation::drawFood(int x, int y)
{
	olc::Pixel lightestGreen{ 186, 246, 117 };
	olc::Pixel lightGreen{ 152, 228, 59};
	olc::Pixel midGreen{ 112, 162, 38 };
	olc::Pixel darkGreen{ 86, 108, 56 };
	olc::Pixel brown{ 115, 62, 57 };
	olc::Pixel white{ 255,255,255,255 };

	int b = m_cellSize / 8;

	//y
	FillRect(x + (4 * b), y, b, b, brown);
	//y+1
	FillRect(x + (3 * b), y+(1*b), b, b, brown);
	//y+2
	FillRect(x + (1 * b), y + (2 * b), b, b, midGreen);
	FillRect(x + (2 * b), y + (2 * b), b, b, lightGreen);
	FillRect(x + (3 * b), y + (2 * b), b, b, brown);
	FillRect(x + (4 * b), y + (2 * b), (2*b), b, lightGreen);
	//y+3
	FillRect(x, y + (3 * b), b, b, darkGreen);
	FillRect(x + b, y + (3 * b), b, b, midGreen);
	FillRect(x + (2 * b), y + (3 * b), (3 * b), b, lightGreen);
	FillRect(x + (5 * b), y + (3 * b), b, b, lightestGreen);
	FillRect(x + (6 * b), y + (3 * b), b, b, lightGreen);
	//y+4
	FillRect(x, y + (4 * b), b, b, darkGreen);
	FillRect(x + b, y + (4 * b), b, b, midGreen);
	FillRect(x + (2 * b), y + (4 * b), (2 * b), b, lightGreen);
	FillRect(x + (4 * b), y + (4 * b), b, b, lightestGreen);
	FillRect(x + (5 * b), y + (4 * b), b, b, white);
	FillRect(x + (6 * b), y + (4 * b), b, b, lightGreen);
	//y+5
	FillRect(x, y + (5 * b), b, b, darkGreen);
	FillRect(x + b, y + (5 * b), b, b, midGreen);
	FillRect(x + (2 * b), y + (5 * b), (5 * b), b, lightGreen);
	//y+6
	FillRect(x, y + (6 * b), (2*b), b, darkGreen);
	FillRect(x + (2*b), y + (6 * b), (5*b), b, midGreen);
	//y+7
	FillRect(x+b, y + (7 * b), (5 * b), b, darkGreen);
}

void Animation::openPipe(FILE* &m_ffmpeg)
{
	std::string cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s " +
		std::to_string(m_xRes) + "x" + std::to_string(m_yRes) +
		" -i - -threads 0 -preset medium -y -pix_fmt yuv420p -crf 18 -r 60 -vf vflip "
		+ m_vidName + ".mp4";
	const char * chrCmd = cmd.c_str();
	// _WIN32 = we're in windows
	#ifdef _WIN32
	m_ffmpeg = _popen(chrCmd, "wb");
	#else
	ffmpeg = popen(cmd, "w");
	#endif
	m_buffer = new int[m_xRes*m_yRes];
}

void Animation::closePipe(FILE* &m_ffmpeg)
{
	#ifdef _WIN32
	_pclose(m_ffmpeg);
	#else
	pclose(m_ffmpeg);
	#endif
	delete m_buffer;
}

bool Animation::OnUserCreate()
{
	//Variables needed during onUserUpdate
	m_eachFoodPositions = m_stats.getEachFoodArray();
	m_dailyBlobFrames = m_stats.getDailyBlobFrames();
	m_avgBlobStats = m_stats.getManySimAvg()[0];
	m_blackBorder = 2;
	m_homeCount = m_gridCount + 2;
	m_screenCount = m_homeCount + (m_blackBorder * 2);
	m_cellSize = ScreenHeight() / m_screenCount;
	m_day = 0;
	m_frame = 0;
	m_scaleRange = 100.0;
	m_xRes = static_cast<int>(ScreenWidth());
	m_yRes = static_cast<int>(ScreenHeight());
	m_paused = false;

	//Check for incorrect user input
	if (m_cellSize < 8)
	{   
		std::cout << "\nMap size too large to display.\n";
		return false;
	}
	if (!colourStatValid())
	{
		m_colourStat = ColourStat::SIZE;
	}
	if (m_vidName.size() == 0)
	{
		m_vidName = getDefaultFilename();
	}

	//Normalise stat values for color bar and blob colors
	scaleStats(m_scaleRange);
	openPipe(m_ffmpeg);
		
	return true;
}

bool Animation::OnUserUpdate(float fElapsedTime)
{
	//Press P key to pause and unpause the animation
	if (GetKey(olc::Key::P).bPressed)
	{
		m_paused = (!m_paused);
	}
	if (m_paused)
	{
		return true;
	}
	
	//Clear Screen
	FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

	//Write Day Number
	std::string title = "Day #" + std::to_string(m_day);
	//Math to scale title size and position with changing grid and border sizes
	int textScale{ (m_blackBorder*m_cellSize / 16) };
	int y{ (m_blackBorder*m_cellSize / 2) - textScale * 3 };
	int x{ ((m_homeCount*m_cellSize / 2) - (textScale * static_cast<int>(title.length()))) };
	DrawString(x, y, title, olc::WHITE, textScale);
	
	//Draw Home
	for (int x = m_blackBorder; x < m_homeCount + m_blackBorder; ++x)
	{
		for (int y = m_blackBorder; y < m_homeCount + m_blackBorder; ++y)
		{
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::GREY);
		}
	}

	//Draw Grid / Map 
	for (int x = m_blackBorder+1; x < m_gridCount + m_blackBorder + 1; ++x)
	{
		for (int y = m_blackBorder + 1; y < m_gridCount + m_blackBorder + 1; ++y)
		{
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::WHITE);
		}
	}

	//Draw Colour Bar
	drawColourBar();

	//Draw Food onto map
	for (Food food : m_eachFoodPositions[m_day])
	{
		drawFood((food.getXPosition() + m_blackBorder)* m_cellSize, (food.getYPosition() + m_blackBorder)* m_cellSize);
	}
	
	//Draw Blobs onto map
	for (auto element : (m_dailyBlobFrames[m_day])[m_frame])
	{
		double scaledSize{ element[m_colourStat] };
		drawBlob(((element[0] + m_blackBorder) * m_cellSize), (element[1] + m_blackBorder) * m_cellSize, scaledSize);

		//If Blob overlaps food, then remove food from foodPosition array
		for (auto it = m_eachFoodPositions[m_day].begin(); it != m_eachFoodPositions[m_day].end();)
		{
			if (it->getXPosition() == element[0] && it->getYPosition() == element[1])
			{
					it = m_eachFoodPositions[m_day].erase(it);

			}
			else
			{
				++it;
			}
		}
	}
	
	//Read the displayed pixels and copy into m_buffer
	glReadPixels(0, 0, m_xRes, m_yRes, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
	//Send m_buffer to ffmpeg via the pipe
	fwrite(m_buffer, sizeof(int)*m_xRes*m_yRes, 1, m_ffmpeg);
	
	++m_frame;

	/*If completed all frames for that day, move onto next day and
	reset the frame counter to zero*/
	if (m_frame == static_cast<int>(m_dailyBlobFrames[m_day].size()))
	{
		//When finished all days, end animation
		if (m_day == m_dailyBlobFrames.size()-1)
		{
			closePipe(m_ffmpeg);
			return false;
		}
		else
		{
			++m_day;
			m_frame = 0;
		}
	}

	// Allows you to limit fps max to stay below ffmpeg input rate
	// std::this_thread::sleep_for(std::chrono::milliseconds(10));

	return true;
}

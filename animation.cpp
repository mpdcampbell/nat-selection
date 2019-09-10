#include <vector>
#include <array>
#include <stdio.h>
#include <math.h>
#include "blob.h"
#include "food.h"
#include "simulationResults.h"
#include "animation.h"

Animation::Animation()
{
	sAppName = "Natural Selection Simulation";
}

Animation::Animation(int cellCount, int framesPerStep, simulationResults &stats, 
		ColourStat colourStat = ColourStat::SIZE)  
		: m_gridCount{ cellCount }, m_interpFrames{ framesPerStep }, m_stats{ stats },
		m_colourStat{ colourStat }
{
	sAppName = "Natural Selection Simulation";
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
				double size = m_dailyBlobFrames[i][j][k][2];
				double speed = m_dailyBlobFrames[i][j][k][3];
				double sense = m_dailyBlobFrames[i][j][k][4];

				m_dailyBlobFrames[i][j][k][2] = size * (scaleRange / maxSizeVal);
				m_dailyBlobFrames[i][j][k][3] = speed * (scaleRange / maxSpeedVal);
				m_dailyBlobFrames[i][j][k][4] = sense * (scaleRange / maxSenseVal);
			}
		}
	}
}

void Animation::interpolateFrames()
{
	if (m_cellSize < m_interpFrames)
	{
		m_interpFrames = m_cellSize;
		std::cout << "Number of interpolated frames entered is greater than the ";
		std::cout<< "number of pixels betwen grid spaces, framesPerStep was set to";
		std::cout << " one pixel increments, framesPerStep = " << m_interpFrames << "\n";
	}
	else if (m_interpFrames == 0)
	{
		return;
	}

	std::vector<std::vector<std::vector<std::array<double, 5>>>> tempDailyFrameArray;
	std::vector< std::vector<std::array<double, 5>>> tempFrameArray;
	
	for (auto day : m_dailyBlobFrames)
	{
		int frameCount{ static_cast<int>(day.size())};
		for (int i{ 0 }; i<frameCount-1; ++i)
		{
			int blobCount{ static_cast<int>(day[i].size()) };
			std::vector<std::array<double, 5>> iFrame = day[i];
			for (int j{ 0 }; j < blobCount; ++j)
			{
				std::array<double, 2> positionOne{ day[i][j][0], day[i][j][1] };
				std::array<double, 2> positionTwo{ day[i+1][j][0], day[i+1][j][1] };				
				if (positionOne != positionTwo)
				{
					if (positionOne[0] != positionTwo[0])
					{
						double increment{ ((positionTwo[0] - positionOne[0]) / m_interpFrames) };
						for (int k{ 0 }; k < m_interpFrames; ++k)
						{
							iFrame[j][0] = positionOne[0] + (increment*k);
							tempFrameArray.push_back(iFrame);
						}						
					}
					else
					{
						double increment{ ((positionTwo[1] - positionOne[1]) / m_interpFrames) };
						for (int k{ 0 }; k < m_interpFrames; ++k)
						{
							iFrame[j][1] = positionOne[1] + (increment*k);
							tempFrameArray.push_back(iFrame);
						}
					}
					j = blobCount;
				}
			}
		}
		tempDailyFrameArray.push_back(tempFrameArray);
		tempFrameArray.clear();
	}
	m_dailyBlobFrames = tempDailyFrameArray;
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

	int i;
	int b = m_cellSize / 8;

	//y
	for (i = 0; i < b; ++i)
	{
		DrawLine(x + (2*b), y + i, x + (6*b), y + i, lightBTR);
	}
	//y+1
	for (i = 0; i < b; ++i)
	{
	DrawLine(x + (1*b), y + (1*b)+i, x + (7*b), y + (1*b)+i, lightBTR);
	}
	//y+2
	for (i = 0; i < b; ++i)
	{
		DrawLine(x, y + (2*b)+i, x + (4*b), y + (2*b)+i, lightBTR);
		DrawLine(x + (4*b), y + (2*b)+i, x + (8*b), y + (2*b)+i, midBTR);
	}
	//y+3
	FillRect(x, y + (3 * b), b, b, lightBTR);
	FillRect(x + (1 * b), y + (3 * b), b, b, midBTR);
	FillRect(x + (2 * b), y + (3 * b), b, b, white);
	for (i = 0; i < b; ++i)
	{
		DrawLine(x + (3*b), y + (3*b)+i, x + (5*b), y + (3*b)+i, midBTR);
	}
	FillRect(x + (6 * b), y + (3 * b), b, b, white);
	for (i = 0; i < b; ++i)
	{
		DrawLine(x + (6*b), y + (3*b)+i, x + (8*b), y + (3*b)+i, midBTR);
	}
	//y+4
	FillRect(x, y + (4 * b), b, b, midBTR);
	FillRect(x + (1 * b), y + (4 * b), b, b, darkBTR);
	FillRect(x + (2*b), y + (4*b),b,b, blackBTR);
	for (i = 0; i < b; ++i)
	{
		DrawLine(x + (3*b), y + (4*b)+i, x + (5*b), y + (4*b)+i, darkBTR);
	}
	FillRect(x + (5 * b), y + (4 * b), b, b, blackBTR);
	for (i = 0; i < b; ++i)
	{
		DrawLine(x + (6*b), y + (4*b)+i, x + (8*b), y + (4*b)+i, midBTR);
	}
	//y+5
	for (i = 0; i < b; ++i)
	{
		DrawLine(x, y + (5*b)+i, x + (2*b), y + (5*b)+i, midBTR);
		DrawLine(x + (2*b), y + (5*b)+i, x + (7*b), y + (5*b)+i, darkBTR);
	}
	FillRect(x + (7 * b)+1, y + (5 * b), b, b, midBTR);
	//y+6
	for (i = 0; i < b; ++i)
	{
		DrawLine(x + (1*b), y + (6*b)+i, x + (7*b), y + (6*b)+i, darkBTR);
	}
	//y+7
	for (i = 0; i < b; ++i)
	{
		DrawLine(x + (2*b), y + (7*b)+i, x + (6*b), y + (7*b)+i, darkBTR);
	}
}

void Animation::drawColourBar()
{
	/*Colour bar changes from red (max) to magenta (mid) to blue (min)
	Red = 228, 59, 68
	Magenta = 228, 59, 152
	Blue = 0, 149, 233 */
	int x = ((m_homeCount + 2 * m_blackBorder)*m_cellSize);
	double step_r{ 0.0 }, step_g{ 0.0 }, step_b{ 0.0 };
	int n{ 0 };
	int yZero{ (m_blackBorder + 1)*m_cellSize }; // Where the colour bar starts
	for (int y = yZero; y < ((m_gridCount + m_blackBorder + 1)*m_cellSize); ++y)
	{
		olc::Pixel RtMtB(228 - step_r, 59 + step_g, 68.0 + step_b);
		DrawLine(x, y, x + (2 * m_cellSize), y, RtMtB);
		++n;

		if (n < (m_gridCount*m_cellSize) / 2)
		{
			step_b += ((152.0 - 68.0) / ((m_gridCount*m_cellSize) / 2));
		}
		else if (n >= (m_gridCount*m_cellSize) / 2)
		{
			step_r += ((228.0) / ((m_gridCount*m_cellSize) / 2));
			step_g += ((149.0 - 59.0) / ((m_gridCount*m_cellSize) / 2));
			step_b += ((233.0 - 152.0) / ((m_gridCount*m_cellSize) / 2));
		}
	}

	//Write which blob stat the colour bar measures
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
	int textScale{ m_blackBorder * m_cellSize / 16 };
	DrawString((x - textScale * 7), (m_blackBorder*m_cellSize / 2), colourStatStr, olc::WHITE, textScale);

	//Write max colour bar value
	std::string maxValStr = std::to_string(m_colourBarMax);
	maxValStr.resize(4);
	textScale = (m_blackBorder*m_cellSize / 20);
	DrawString((x - textScale * 3), ((m_blackBorder + 1)*m_cellSize - textScale * 9), maxValStr, olc::WHITE, textScale);

	//Write min colour bar value
	DrawString(x - textScale * 3, (m_gridCount + m_blackBorder + 1)*m_cellSize + textScale * 2, "0.00", olc::WHITE, textScale);

	// Draw mean value triangle
	int avgStatIndex{ static_cast<int>(m_colourStat) - 1 }; //index of average colour stat in m_avgBlobStats
	double avgStatVal{ (m_avgBlobStats[m_day-1][avgStatIndex]) };
	double yMean = yZero + (1 - avgStatVal / m_colourBarMax)*(m_gridCount*m_cellSize); //where along colour bar the daily mean value is
	FillTriangle(x + (2.25* m_cellSize), yMean, x + (3.25*m_cellSize), yMean - (0.5*m_cellSize), x + (3.25*m_cellSize), yMean + (0.5*m_cellSize), olc::WHITE);
	DrawString(x + (3.50 * m_cellSize), yMean - (0.75*m_cellSize), "Mean", olc::WHITE, textScale);
	std::string avgValStr{ std::to_string(avgStatVal) };
	avgValStr.resize(4);
	DrawString(x + (3.50 * m_cellSize), yMean, avgValStr, olc::WHITE, textScale);

	// Draw max value triangle
	int maxStatIndex{ static_cast<int>(m_colourStat) + 2 }; //index of max colour stat in m_avgBlobStats
	double maxStatVal{ (m_avgBlobStats[m_day-1][maxStatIndex]) };
	int yMax = yZero + (1 - maxStatVal / m_colourBarMax)*(m_gridCount*m_cellSize); //where along colour bar the daily max value is
	//if the max val triangle doesn't overlap mean triangle, draw
	if (std::abs(yMean - yMax) >= m_cellSize)
	{
		FillTriangle(x + (2.25* m_cellSize), yMax, x + (3.25 * m_cellSize), yMax - (0.5*m_cellSize), x + (3.25 * m_cellSize), yMax + (0.5*m_cellSize), olc::WHITE);
		DrawString(x + (3.50 * m_cellSize), yMax - (0.75*m_cellSize), "Max", olc::WHITE, textScale);
		std::string dailyMaxStr{ std::to_string(maxStatVal) };
		dailyMaxStr.resize(4);
		DrawString(x + (3.50 * m_cellSize), yMax, dailyMaxStr, olc::WHITE, textScale);
	}

	// Draw min value triangle
	int minStatIndex{ static_cast<int>(m_colourStat) + 5 }; //index of min colour stat in m_avgBlobStats
	double minStatVal{ (m_avgBlobStats[m_day-1][minStatIndex]) };
	int yMin = yZero + (1 - minStatVal / m_colourBarMax)*(m_gridCount*m_cellSize); //where along colour bar the daily min value is
	//if the min val triangle doesn't overlap mean triangle, draw
	if (std::abs(yMean - yMin) >= m_cellSize)
	{
		FillTriangle(x + (2.25* m_cellSize), yMin, x + (3.25 * m_cellSize), yMin - (0.5*m_cellSize), x + (3.25 * m_cellSize), yMin + (0.5*m_cellSize), olc::WHITE);
		DrawString(x + (3.50 * m_cellSize), yMin - (0.75*m_cellSize), "Min", olc::WHITE, textScale);
		std::string dailyMinStr{ std::to_string(minStatVal) };
		dailyMinStr.resize(4);
		DrawString(x + (3.50 * m_cellSize), yMin, dailyMinStr, olc::WHITE, textScale);
	}
}

bool Animation::OnUserCreate()
{
	m_eachFoodPositions = m_stats.getEachFoodArray();
	m_dailyBlobFrames = m_stats.getDailyBlobFrames();
	m_avgBlobStats = m_stats.getManySimAvg()[0];
	m_blackBorder = 2;
	m_homeCount = m_gridCount + 2;
	m_screenCount = m_homeCount + (m_blackBorder * 2);
	m_cellSize = ScreenHeight() / m_screenCount;
	m_day = 1;
	m_frame = 0;
	m_scaleRange = 100.0;
	m_paused = false;

	if (m_cellSize < 8)
	{
		std::cout << "\nMap size too large to display, increase window resolution or decrease map size.\n\n";
		return false;
	}
	
	scaleStats(m_scaleRange);
	interpolateFrames();

	const char* cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 900x600 -i - "
		"-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip toutput.mp4";
	m_ffmpeg = _popen(cmd, "wb");
	m_width = static_cast<int>(ScreenWidth());
	m_height = static_cast<int>(ScreenHeight());
	m_buffer = new int[m_width*m_height];
	
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
		{;
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::WHITE);
		}
	}

	//Draw Colour Bar
	drawColourBar();

	//Draw Food onto map
	std::string sImageFile = "greenApple.png";
	olc::Sprite greenApple(sImageFile);
	int appleScale{ m_cellSize / 8 };
	for (Food food : m_eachFoodPositions[m_day])
	{
		DrawSprite((food.getXPosition() + m_blackBorder)* m_cellSize, (food.getYPosition() + m_blackBorder)* m_cellSize, &greenApple, appleScale);
	}
	
	//Draw Blobs onto map
	for (auto element : (m_dailyBlobFrames[m_day])[m_frame])
	{
		int index{ static_cast<int>(m_colourStat) };
		double scaledSize{ element[index] };
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
	glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
	//send m_buffer to ffmpeg via the pipe "m_ffmpeg"
	fwrite(m_buffer, sizeof(int)*m_width*m_height, 1, m_ffmpeg);
	
	++m_frame;

	/*If completed all frames for that day, move onto next day and
	reset the frame counter to zero*/
	if (m_frame == static_cast<int>(m_dailyBlobFrames[m_day].size()))
	{
		++m_day;
		m_frame = 0;
	}

	//When finished all days, end animation
	if (m_day == m_dailyBlobFrames.size())
	{
		_pclose(m_ffmpeg);
		return false;
	}

	/*Can be used as a delay between each step if your computer is too fast
	and the animation runs too quickly*/

	//int num = fElapsedTime;
	//std::this_thread::sleep_for(std::chrono::seconds(num));

	return true;
}

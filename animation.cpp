#include <vector>
#include <array>
#include "blob.h"
#include "food.h"
#include "simulationResults.h"
#include "animation.h"

Animation::Animation()
{
	sAppName = "Natural Selection Simulation";
}

Animation::Animation(int cellCount, simulationResults &stats)
	: m_gridCount{ cellCount }, m_stats{ stats }
{
	sAppName = "Natural Selection Simulation";
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
	m_cellBorderWidth = 1;
	m_day = 1;
	m_frame = 0;

	if (m_cellSize < 5)
	{
		std::cout << "mapSize too large to display, each grid space is smaller than a single pixel.\n";
		return false;
	}

	fixCoords();
	scaleStats(0,255);
	return true;
}

void Animation::fixCoords()
{
	int n{ m_homeCount - 1 };
	for (int i{ 0 }; i < m_dailyBlobFrames.size(); ++i)
	{
		for (int j{ 0 }; j < m_dailyBlobFrames[i].size(); ++j)
		{
			for (int k{ 0 }; k < (m_dailyBlobFrames[i])[j].size(); ++k)
			{
				double x = m_dailyBlobFrames[i][j][k][0];
				double y = m_dailyBlobFrames[i][j][k][1];

				/*Rotates each point 90 degrees clockwise about origin,
				then shifts upwards by n. This converts mycartesion system
				to the matrix system used in the animation library*/
				m_dailyBlobFrames[i][j][k][0] = (y);
				m_dailyBlobFrames[i][j][k][1] = -x + n;
			}
		}
	}
}

void Animation::drawBlob(int x, int y, double s)
{
	double t = s / 255.0;
	olc::Pixel lightBTR(44.0+(t*202.0), 232.0 - (t*115.0), (245.0 -(t*123.0)));
	olc::Pixel midBTR(t*228.0, 149.0 - (t * 90.0), 233.0 - (t * 165));
	olc::Pixel darkBTR(18.0 + (t * 140), 78.0 - (t * 38.0), 137.0 - (t*84.0));
	olc::Pixel blackBTR(38.0 + (t * 30), 43.0 + (t*6.0), 68.0 - (t * 30.0));
	olc::Pixel white(255, 255, 255, 255);
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

	/*
	//y
	DrawLine(x + 2, y, x + 5, y, lightBTR);
	//y+1
	DrawLine(x + 1, y + 1, x + 6, y + 1, lightBTR);
	//y+2
	DrawLine(x, y + 2, x + 3, y + 2, lightBTR);
	DrawLine(x + 4, y + 2, x + 7, y + 2, midBTR);
	//y+3
	Draw(x, y + 3, lightBTR);
	Draw(x + 1, y + 3, midBTR);
	Draw(x + 2, y + 3, olc::WHITE);
	DrawLine(x + 3, y + 3, x + 4, y + 3, midBTR);
	Draw(x + 5, y + 3, olc::WHITE);
	DrawLine(x + 6, y + 3, x + 7, y + 3, midBTR);
	//y+4
	Draw(x, y + 4, midBTR);
	Draw(x + 1, y + 4, darkBTR);
	Draw(x + 2, y + 4, blackBTR);
	DrawLine(x + 3, y + 4, x + 4, y + 5, darkBTR);
	Draw(x + 5, y + 4, blackBTR);
	DrawLine(x + 6, y + 4, x + 7, y + 4, midBTR);
	//y+5
	DrawLine(x, y + 5, x + 1, y + 5, midBTR);
	DrawLine(x + 2, y + 5, x + 6, y + 5, darkBTR);
	Draw(x + 7, y + 5, midBTR);
	//y+6
	DrawLine(x + 1, y + 6, x + 6, y + 6, darkBTR);
	//y+7
	DrawLine(x + 2, y + 7, x + 5, y + 7, darkBTR);
	*/
}


void Animation::scaleStats(int start, int end)
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

	for (int i{ 0 }; i < m_dailyBlobFrames.size(); ++i)
	{
		for (int j{ 0 }; j < m_dailyBlobFrames[i].size(); ++j)
		{
			for (int k{ 0 }; k < (m_dailyBlobFrames[i])[j].size(); ++k)
			{
				double size = m_dailyBlobFrames[i][j][k][2];
				double speed = m_dailyBlobFrames[i][j][k][3];
				double sense = m_dailyBlobFrames[i][j][k][4];

				m_dailyBlobFrames[i][j][k][2] = size * (end / maxSizeVal);
				m_dailyBlobFrames[i][j][k][3] = speed * (end / maxSpeedVal);
				m_dailyBlobFrames[i][j][k][4] = sense * (end / maxSenseVal);
			}
		}
	}
}


bool Animation::OnUserUpdate(float fElapsedTime)
{
	//std::cout << "Day: " << m_day << "\n";
	if (GetKey(olc::Key::SPACE).bHeld)
	{
		return true;
	}

	//Clear Screen
	FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

	//Write Day Number
	{
		std::string title = "Day #" + std::to_string(m_day);
		//Math to scale title size and position with changing grid sizes
		int scale{ (m_blackBorder*m_cellSize / 16) };
		int y{ (m_blackBorder*m_cellSize / 2) - scale * 3 };
		int x{ ((m_homeCount*m_cellSize / 2) - (scale * 8)) };
		DrawString(x, y, title, olc::WHITE, scale);
	}
	
	//Draw Home hopefully
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
			//FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::BLACK);
			//FillRect(x * m_cellSize, y * m_cellSize, m_cellSize - m_cellBorderWidth, m_cellSize - m_cellBorderWidth, olc::WHITE);
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::WHITE);
		}
	}

	/*
	//Draw Colour Bar in cells
	for (int x = m_homeCount + 2*m_blackBorder; x < m_homeCount + 2*m_blackBorder + 2; ++x)
	{
		int step{ 0 };
		for (int y = m_blackBorder+1; y < m_gridCount + m_blackBorder; ++y)
		{
			olc::Pixel redToBlue((255 - step), 0, step);
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize + m_cellBorderWidth, m_cellSize + m_cellBorderWidth, olc::WHITE);
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, redToBlue);
			step += (255 / m_homeCount);
		}
	}
	*/

	//Draw Colour Bar in lines
	{
		/*Colour bar changes from pastel red (max) to pastel blue (min)
		Pastel Red = 255, 59, 48
		Pastel Blue = 0, 122, 255
		Code is uglier but result prettier*/
		int x = ((m_homeCount + 2 * m_blackBorder)*m_cellSize);
		double step_r{ 0.0 }, step_g{ 0.0 }, step_b{ 0.0 };
		for (int y = (m_blackBorder + 1)*m_cellSize; y <= (m_gridCount + m_blackBorder+1)*m_cellSize; ++y)
		{
			//olc::Pixel redToBlue((255 - step), 0, (step));
			//olc::Pixel pastelRTB((255 - step_r), (59+step_g), (48+step_b));
			olc::Pixel blobRTB(228 - step_r, 59 + step_g, 68 + step_b);
			DrawLine(x, y, x + (2 * m_cellSize), y, blobRTB);
			//step_r = step_r + (255.0 / (m_gridCount*m_cellSize));
			//step_g = step_g + ((122.0-59.0) / (m_gridCount*m_cellSize));
			//step_b = step_b + (207.0 / (m_gridCount*m_cellSize));
			
			//Matches Blob sprites
			step_r = step_r + (228.0 / (m_gridCount*m_cellSize));
			step_g = step_g + ((149.0 - 59.0) / (m_gridCount*m_cellSize));
			step_b = step_b + ((233.0-68.0) / (m_gridCount*m_cellSize));
		}

	}

	//Draw Food onto map
	std::string sImageFile = "greenApple.png";
	olc::Sprite greenApple(sImageFile);
	int appleScale{ m_cellSize / 8 };
	for (Food food : m_eachFoodPositions[m_day])
	{
		//olc::Pixel green_custom{ 34, 139, 34, 0};//190
		//FillCircle((food.getXPosition() + m_blackBorder+0.5)* m_cellSize, (food.getYPosition() + m_blackBorder+0.5)* m_cellSize, 0.4*m_cellSize, green_custom);
		DrawSprite((food.getXPosition() + m_blackBorder)* m_cellSize, (food.getYPosition() + m_blackBorder)* m_cellSize, &greenApple, appleScale);
	}

	
	//Draw Blobs onto map
	sImageFile = "redBlob.png";
	olc::Sprite redBlob (sImageFile);
	sImageFile = "blueBlob.png";
	olc::Sprite blueBlob(sImageFile);

	for (auto element : (m_dailyBlobFrames[m_day])[m_frame])
	{
		/*Scaling function scales to 255, allows for pretty code here but
		pure red to pure blue is ugly. Pastel to Pastel is nicer*/

		double scaledSize{ (element[2]) };
		//olc::Pixel blueToRed(scaledSize, 0, (255 - scaledSize));
		//olc::Pixel pastelBTR(scaledSize, 122-(scaledSize/255*63.0), (255 - (scaledSize / 255 * 207.0)));
		//FillRect((element[0] + m_blackBorder) * m_cellSize, (element[1] + m_blackBorder) * m_cellSize, m_cellSize - m_cellBorderWidth, m_cellSize - m_cellBorderWidth, pastelBTR);
		//DrawSprite(((element[0] + m_blackBorder) * m_cellSize), (element[1] + m_blackBorder) * m_cellSize, &redBlob, 3);
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
		return false;
	}

	/*Can be used as a delay between each step if your computer is too fast
	and the animation runs too quickly*/
	//std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return true;
}

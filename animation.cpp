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
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::BLACK);
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize - m_cellBorderWidth, m_cellSize - m_cellBorderWidth, olc::WHITE);
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
		int x = ((m_homeCount + 2 * m_blackBorder)*m_cellSize);
		double step{ 0.0 };
		int numLinesDrawn{ 0 };
		for (int y = (m_blackBorder + 1)*m_cellSize; y <= (m_gridCount + m_blackBorder)*m_cellSize; ++y)
		{
			olc::Pixel redToBlue((255 - (step)), 0, (step));
			DrawLine(x, y, x + (2 * m_cellSize), y, redToBlue);
			++numLinesDrawn;
			step = step + (255.0 / (m_gridCount*m_cellSize));
		}

		
	}

	//Draw Food onto map
	for (Food food : m_eachFoodPositions[m_day])
	{
		olc::Pixel green_custom{ 34, 139, 34, 127};
		FillCircle((food.getXPosition() + m_blackBorder+0.5)* m_cellSize, (food.getYPosition() + m_blackBorder+0.5)* m_cellSize, 0.4*m_cellSize, green_custom);
	}

	//Draw Blobs onto map
	for (auto element : (m_dailyBlobFrames[m_day])[m_frame])
	{
		int scaledSize{ static_cast<int>(element[2]) };
		olc::Pixel blueToRed(scaledSize, 0, (255-scaledSize));
		FillRect((element[0] + m_blackBorder) * m_cellSize, (element[1] + m_blackBorder) * m_cellSize, m_cellSize - m_cellBorderWidth, m_cellSize - m_cellBorderWidth, blueToRed);
		
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

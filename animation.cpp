#include <vector>
#include <array>
#include "blob.h"
#include "food.h"
#include "animation.h"

Animation::Animation()
{
	sAppName = "Natural Selection Simulation";
}

Animation::Animation(int cellCount, std::vector<std::vector<Food>> &eachFoodPositions,
	std::vector<std::vector<std::vector<std::array<int, 2>>>> &dailyBlobFrames)
	: m_gridCount{ cellCount }, m_eachFoodPositions{ eachFoodPositions },
	m_dailyBlobFrames{ dailyBlobFrames }
{

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
				int x = m_dailyBlobFrames[i][j][k][0];
				int y = m_dailyBlobFrames[i][j][k][1];

				/*Rotates each point 90 degrees clockwise about origin,
				then shifts upwards by n. This converts mycartesion system
				to the matrix system used in the animation library*/
				m_dailyBlobFrames[i][j][k][0] = (y);
				m_dailyBlobFrames[i][j][k][1] = -x + n;
			}
		}
	}
}

bool Animation::OnUserCreate()
{
	m_screenCount = m_gridCount + 15;
	m_homeCount = m_gridCount + 2;
	m_cellSize = ScreenHeight() / m_screenCount;
	m_borderWidth = 1;
	m_day = 1;
	m_frame = 0;

	fixCoords();
	return true;
}


bool Animation::OnUserUpdate(float fElapsedTime)
{
	std::cout << "Day: " << m_day << "\n";
	if (GetKey(olc::Key::SPACE).bHeld)
	{
		return true;
	}

	//Clear Screen
	FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);


	//Draw Home hopefully
	olc::Pixel grey{ 192, 192, 192 };
	for (int x = 1; x < m_gridCount + 3; ++x)
	{
		for (int y = 1; y < m_gridCount + 3; ++y)
		{

			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, grey);
		}
	}

	//Draw Grid / Map 
	for (int x = 2; x < m_gridCount + 2; ++x)
	{
		for (int y = 2; y < m_gridCount + 2; ++y)
		{
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::BLACK);
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize - m_borderWidth, m_cellSize - m_borderWidth, olc::WHITE);
		}
	}

	for (Food food : m_eachFoodPositions[m_day])
	{
		olc::Pixel green_50{ 0, 255, 0, 128 };
		FillRect((food.getXPosition() + 1)* m_cellSize, (food.getYPosition() + 1)* m_cellSize, m_cellSize - m_borderWidth, m_cellSize - m_borderWidth, green_50);
	}

	for (auto position : (m_dailyBlobFrames[m_day])[m_frame])
	{
		olc::Pixel blue_50{ 0, 0, 255, 128 };
		FillRect((position[0] + 1) * m_cellSize, (position[1] + 1) * m_cellSize, m_cellSize - m_borderWidth, m_cellSize - m_borderWidth, blue_50);
		for (auto it = m_eachFoodPositions[m_day].begin(); it != m_eachFoodPositions[m_day].end();)
		{
			if (it->getXPosition() == position[0] && it->getYPosition() == position[1])
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

	if (m_frame == static_cast<int>(m_dailyBlobFrames[m_day].size()))
	{
		++m_day;
		m_frame = 0;
	}

	if (m_day == m_dailyBlobFrames.size())
	{
		return false;
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	return true;
}

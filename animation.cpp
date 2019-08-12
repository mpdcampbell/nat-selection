/*

#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include "blob.h"
#include "animation.h"

Animation::Animation()
{
	m_sAppName = L"Natural Selection Simulation";
}
	
	Animation::Animation(int cellCount, std::vector<std::vector<Food>> &eachFoodPositions)
		: m_cellCount{ cellCount }, m_eachFoodPositions{ eachFoodPositions }
	{

	}

	bool Animation::OnUserCreate()
	{
		m_day = 0;
		return true;
	}


	/*
	bool Animation::playAnimation()
	{
		int day{ 0 };
		do {
			bool stepTaken{ false };
			int step{ 0 };
			do {
				for (auto it = m_allBlobs.begin(); it != m_allBlobs.end();)
				{
					if (it->getBirthday() <= day)
					{
						std::vector<std::array<int, 2>> path{ it->getPath() };
						if (step <= it->getSpeed())
						{
							//std::array<int, 2> position{ path[day][step] };
							std::array<int, 2> position{ path[step] };
							olc::Pixel blue_50{ 0, 0, 255, 128 };
							FillRect(position[0]*m_cellSize, position[1] * m_cellSize, m_cellSize - nBorderWidth, m_cellSize - nBorderWidth, blue_50);
							stepTaken = true;
						}
						if (path.size() == day)
						{
							it = m_allBlobs.erase(it);
						}
						else
						{
							++it;
						}
					
					}
					else
					{
						++it;
					}
				}
				//return true;
				++step;
			} while (stepTaken);
			++day;
		} while (m_allBlobs.size() > 0);
	}
	*/
/*
	bool Animation::OnUserUpdate(float fElapsedTime)
	{

		if (m_keys[VK_SPACE].bHeld)
		{
			return true;
		}

		//Draw grid hopefully

		for (int x = 0; x < m_cellCount; x++)
		{
			for (int y = 0; y < m_cellCount; y++)
			{
				//olc::Pixel colour = olc::BLUE;

				// Draw Base
				Draw(x, y, PIXEL_SOLID, FG_WHITE);
			}
		}

		for (Food food : m_eachFoodPositions[m_day])
		{
			Draw(food.getXPosition(), food.getYPosition(), PIXEL_SOLID, FG_DARK_GREEN);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		return true;
	}
	*/
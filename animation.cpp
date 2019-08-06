#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include "blob.h"
#include "olcPixelGameEngine.h"
#include "animation.h"

bool sortByName(Blob &x, Blob &y)
{
	return (x.getName()) < (y.getName());
}

std::vector<Blob> combineBlobArrays(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray)
{
	std::vector<Blob> allBlobs{ blobArray };
	allBlobs.insert(allBlobs.end(), deadBlobArray.begin(), deadBlobArray.end());
	std::sort(allBlobs.begin(), allBlobs.end(), sortByName);
	return allBlobs;
}

	BlobSim::BlobSim()
	{
		sAppName = "Natural Selection Simulation";
	}
	
	BlobSim::BlobSim(int cellCount, std::vector<Blob> &allBlobs)
		: m_cellCount{ cellCount }, m_allBlobs{ allBlobs }
	{

	}

	/*
	void setBlobArray(std::vector<Blob> &allBlobs)
	{
		m_allBlobs = allBlobs;
	}
	*/

	bool BlobSim::OnUserCreate()
	{
		nBorderWidth = 1;
		nMapWidth = m_cellCount;
		nMapHeight = m_cellCount;
		m_cellSize = ScreenWidth() / m_cellCount;
		return true;
	}

	void BlobSim::playAnimation()
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
							std::array<int, 2> position{ path[day][step] };
							olc::Pixel blue_50{ 0, 0, 255, 128 };
							FillRect(position[0], position[1], m_cellSize - nBorderWidth, m_cellSize - nBorderWidth, blue_50);
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
				++step;
			} while (stepTaken);
			++day;
		} while (m_allBlobs.size() > 0);
	}

	bool BlobSim::OnUserUpdate(float fElapsedTime) 
	{
		if (GetKey(olc::Key::SPACE).bHeld)
		{
			return true;
		}

		// Draw Map
		Clear(olc::BLACK);

		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				//olc::Pixel colour = olc::BLUE;

				// Draw Base
				FillRect(x * m_cellSize, y * m_cellSize, m_cellSize - nBorderWidth, m_cellSize - nBorderWidth, olc::WHITE);
			}
		}
		playAnimation();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		return true;
	}


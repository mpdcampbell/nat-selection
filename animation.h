#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include "blob.h"
/*
#include "olcPixelGameEngine.h"

class Animation : public olc::PixelGameEngine
{
public:
	Animation();

private:
	int nMapWidth;
	int nMapHeight;
	int m_cellCount;
	int m_cellSize;
	int nBorderWidth;
	int cellPos = 1;
	std::vector<std::vector<std::array<int, 2>>> m_eachDaysSteps;
	std::vector<Food> foodArray;

public:
	Animation(int cellCount, std::vector<std::vector<std::array<int,2>>> &eachDaysSteps);


	void setBlobArray(std::vector<Blob> &allBlobs)
	{
		m_allBlobs = allBlobs;
	}
	

	bool OnUserCreate() override;

	bool playAnimation();

	bool OnUserUpdate(float fElapsedTime) override;
};
*/

#endif

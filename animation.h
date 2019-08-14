#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <array>
#include "blob.h"
#include "food.h"
#include "olcPixelGameEngine.h"

class Animation : public olc::PixelGameEngine
{
private:
	int m_screenCount;
	int m_homeCount;
	int m_gridCount;
	int m_cellSize;
	int m_borderWidth;
	int m_day;
	int m_frame;
	std::vector<std::vector<Food>> m_eachFoodPositions;
	std::vector<std::vector<std::vector<std::array<int, 2>>>> m_dailyBlobFrames;

public:
	Animation();

	Animation(int cellCount, std::vector<std::vector<Food>> &eachFoodPositions,
		std::vector<std::vector<std::vector<std::array<int, 2>>>> &dailyBlobFrames);

	void fixCoords();

	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;
};

#endif

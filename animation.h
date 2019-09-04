#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <array>
#include "blob.h"
#include "food.h"
#include "simulationResults.h"
#include "olcPixelGameEngine.h"

enum class ColourStat
{
	SIZE = 2,
	SPEED = 3,
	SENSE = 4
};


class Animation : public olc::PixelGameEngine
{
private:
	int m_blackBorder;
	int m_screenCount;
	int m_homeCount;
	int m_gridCount;
	int m_cellSize;
	int m_day;
	int m_frame;
	double m_scaleRange;
	bool m_paused;
	ColourStat m_colourStat;
	double m_colourBarMax;
	simulationResults m_stats;
	std::vector<std::vector<Food>> m_eachFoodPositions;
	std::vector<std::vector<std::vector<std::array<double, 5>>>> m_dailyBlobFrames;
	std::vector<std::array<double, 10>> m_avgBlobStats;

public:
	Animation();

	Animation(int cellCount, simulationResults &stats, ColourStat stat );

	void fixCoords();

	void drawColourBar();

	void scaleStats(double scaleRange);

	void interpolateFrames(int numFrames);

	void drawBlob(int x, int y, double scaledStat);

	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;
};

#endif

#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <array>
#include <stdio.h>
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
	int m_interpFrames;
	double m_scaleRange;
	double m_colourBarMax;
	bool m_paused;
	ColourStat m_colourStat;
	simulationResults m_stats;
	std::vector<std::vector<Food>> m_eachFoodPositions;
	std::vector<std::vector<std::vector<std::array<double, 6>>>> m_dailyBlobFrames;
	std::vector<std::array<double, 10>> m_avgBlobStats;

	FILE* m_ffmpeg;
	int* m_buffer;
	int m_xRes;
	int m_yRes;
	std::string m_vidName;

public:
	Animation();

	Animation(int cellCount, int framesPerStep, simulationResults &stats, 
			std::string vidName, ColourStat stat );

	bool colourStatValid();

	std::string getColourStatStr();

	std::string getDefaultFilename();

	void scaleStats(double scaleRange);

	void interpolateFrames();

	void drawColourBar();
	   
	void drawBlob(int x, int y, double scaledStat);

	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;
};

#endif

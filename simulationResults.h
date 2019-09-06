#ifndef SIMULATIONRESULTS_H
#define SIMULATIONRESULTS_H

#include <array>
#include <vector>
#include <algorithm>
#include "blob.h"
#include "food.h"

double getMax(std::vector<double> &vector);

class simulationResults
{
protected:
	int simCounter{ 0 };
	std::vector<std::array<double, 10>> m_avgBlobStats;
	std::vector<std::vector<std::vector<double>>> m_eachBlobStats;
	std::vector<std::vector<std::array<double, 10>>> m_manySimAvg;
	std::vector<std::vector<std::vector<std::vector<double>>>> m_manySimEach;

	std::vector<std::array<double, 6>> m_blobFrame;
	std::vector<std::vector<std::array<double, 6>>> m_blobFrameArray;
	std::vector<std::vector<std::vector<std::array<double, 6>>>> m_dailyBlobframes;
	std::vector<std::vector<Food>> m_eachFoodArray;

public:

	simulationResults();

	void recordAvgBlobStats(std::vector<Blob> &blobArray);

	void recordEachBlobStats(std::vector<Blob> &blobArray);

	void recordBlobFrame(std::vector<Blob> &blobArray);

	void pushBlobFrames();

	void recordFoodPositions(std::vector<Food> &foodArray);

	void recordDay(std::vector<Blob> &blobArray, std::vector<Food> &foodArray);

	void recordSim();

	std::vector<std::array<double, 10>>& getAvgBlobStats();

	std::vector<std::vector<std::vector<double>>>& getEachBlobStats();

	std::vector<std::vector<std::array<double, 10>>>& getManySimAvg();

	std::vector<std::vector<std::vector<std::vector<double>>>>& getManySimEach();

	std::vector<std::vector<Food>>& getEachFoodArray();

	std::vector<std::vector<std::vector<std::array<double, 6>>>>& simulationResults::getDailyBlobFrames();
};

#endif

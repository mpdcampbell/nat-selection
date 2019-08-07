#ifndef SIMULATIONRESULTS_H
#define SIMULATIONRESULTS_H

#include <array>
#include <vector>
#include <algorithm>
#include "blob.h"

bool sortByName(Blob &x, Blob &y);

class simulationResults
{
protected:
	std::vector<std::array<double, 10>> m_avgBlobStats;
	std::vector<std::vector<std::vector<double>>> m_eachBlobStats;
	std::vector<std::vector<std::array<double, 10>>> m_manySimAvg;
	std::vector<std::vector<std::vector<std::vector<double>>>> m_manySimEach;

	std::vector<std::array<int, 2>> m_daysSteps;
	std::vector<std::vector<std::array<int, 2>>> m_eachDaysSteps;

public:

	simulationResults();

	void recordAvgBlobStats(std::vector<Blob> &blobArray);

	void recordEachBlobStats(std::vector<Blob> &blobArray);

	std::vector<Blob> combineBlobArrays(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray);

	void recordDaysSteps(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray);

	void recordDay(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray);

	void recordSim();

	std::vector<std::array<double, 10>> getAvgBlobStats();

	std::vector<std::vector<std::vector<double>>> getEachBlobStats();

	std::vector< std::vector<std::array<double, 10>>> getManySimAvg();

	std::vector<std::vector<std::vector<std::vector<double>>>> getManySimEach();

	std::vector<std::array<int, 2>> getDaysSteps();

	std::vector<std::vector<std::array<int, 2>>> getEachDaysSteps();
};

#endif

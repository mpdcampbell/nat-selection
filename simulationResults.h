#ifndef SIMULATIONRESULTS_H
#define SIMULATIONRESULTS_H

#include <array>
#include <vector>
#include "blob.h"


class simulationResults
{
protected:
	std::vector<std::array<double, 10>> m_avgBlobStats;
	std::vector<std::vector<std::vector<double>>> m_eachBlobStats;
	std::vector<std::vector<std::array<double, 10>>> m_manySimAvg;
	std::vector<std::vector<std::vector<std::vector<double>>>> m_manySimEach;

public:

	void recordAvgBlobStats(std::vector<Blob> &blobArray);

	void recordEachBlobStats(std::vector<Blob> &blobArray);

	void recordDay(std::vector<Blob> &blobArray);

	void recordSim();

	std::vector<std::array<double, 10>> getAvgBlobStats();

	std::vector<std::vector<std::vector<double>>> getEachBlobStats();

	std::vector< std::vector<std::array<double, 10>>> getManySimAvg();

	std::vector<std::vector<std::vector<std::vector<double>>>> getManySimEach();
};

#endif

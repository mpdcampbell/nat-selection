#include <array>
#include <vector>
#include "blob.h"
#include "simulationResults.h"

void simulationResults::recordAvgBlobStats(std::vector<Blob> &blobArray)
{
	int pop{ static_cast<int>(blobArray.size()) };
	double sumSize{ 0 }, sumSpeed{ 0 }, sumSense{ 0 };
	double maxSize{ 0 }, maxSpeed{ 0 }, maxSense{ 0 };
	double minSize{ 100 }, minSpeed{ 100 }, minSense{ 100 };

	for (Blob blob : blobArray)
	{
		sumSize += blob.getSize();
		sumSpeed += blob.getSpeed();
		sumSense += blob.getSense();

		if (maxSize < blob.getSize())
		{
			maxSize = blob.getSize();
		}
		if (maxSpeed < blob.getSpeed())
		{
			maxSpeed = blob.getSpeed();
		}
		if (maxSense < blob.getSense())
		{
			maxSense = blob.getSense();
		}
		if (minSize > blob.getSize())
		{
			minSize = blob.getSize();
		}
		if (minSpeed > blob.getSpeed())
		{
			minSpeed = blob.getSpeed();
		}
		if (minSense > blob.getSense())
		{
			minSense = blob.getSense();
		}
	}

	double avgSize{ sumSize / pop };
	double avgSpeed{ sumSpeed / pop };
	double avgSense{ sumSense / pop };

	std::array<double, 10> avgStatArray{ pop, avgSize, avgSpeed, avgSense,
		maxSize, maxSpeed, maxSense, minSize, minSpeed, minSense };

	m_avgBlobStats.push_back(avgStatArray);
}

void simulationResults::recordEachBlobStats(std::vector<Blob> &blobArray)
{
	std::vector<double> speedList;
	std::vector<double> sizeList;
	std::vector<double> senseList;
	//only holds single int but easiest way too pass through population information
	std::vector<double> popVector;
	double pop{ static_cast<double>(blobArray.size()) };
	for (Blob blob : blobArray)
	{
		sizeList.push_back(blob.getSize());
		speedList.push_back(blob.getSpeed());
		senseList.push_back(blob.getSense());
	}
	popVector.push_back(pop);
	std::vector<std::vector<double>> dayVector{ popVector, sizeList, speedList, senseList };
	m_eachBlobStats.push_back(dayVector);
}

void simulationResults::recordDay(std::vector<Blob> &blobArray)
{
	recordAvgBlobStats(blobArray);
	recordEachBlobStats(blobArray);
}

void simulationResults::recordSim()
{
	m_manySimAvg.push_back(m_avgBlobStats);
	m_manySimEach.push_back(m_eachBlobStats);
	m_avgBlobStats.clear();
	m_eachBlobStats.clear();
}

std::vector<std::array<double, 10>> simulationResults::getAvgBlobStats()
{
	return m_avgBlobStats;
}

std::vector<std::vector<std::vector<double>>> simulationResults::getEachBlobStats()
{
	return m_eachBlobStats;
}

std::vector< std::vector<std::array<double, 10>>> simulationResults::getManySimAvg()
{
	return m_manySimAvg;
}

std::vector<std::vector<std::vector<std::vector<double>>>> simulationResults::getManySimEach()
{
	return m_manySimEach;
}
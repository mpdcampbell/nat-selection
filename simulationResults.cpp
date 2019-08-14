#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "blob.h"
#include "simulationResults.h"

double getMax(std::vector<double> &vector)
{
	double max = vector[0];
	for (int i = 0; i < vector.size(); i++)
	{
		if (max < vector[i])
		{
			max = vector[i];
		}
	}
	return max;
}

simulationResults::simulationResults()
{
}

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

void simulationResults::recordBlobFrame(std::vector<Blob> &blobArray)
{
	if (simCounter == 0)
	{
		for (Blob blob : blobArray)
		{
			std::array<double, 5> posAndStats{ blob.getXPosition(), blob.getYPosition(), blob.getSize(),
								blob.getSpeed(), blob.getSense()};
			m_blobFrame.push_back(posAndStats);
		}

		m_blobFrameArray.push_back(m_blobFrame);
		m_blobFrame.clear();
	}
}

void simulationResults::pushBlobFrames()
{
	m_dailyBlobframes.push_back(m_blobFrameArray);
	m_blobFrameArray.clear();
}

void simulationResults::recordFoodPositions(std::vector<Food> &foodArray)
{
	m_eachFoodArray.push_back(foodArray);
}

void simulationResults::recordDay(std::vector<Blob> &blobArray, std::vector<Food> &foodArray)
{
	recordAvgBlobStats(blobArray);
	recordEachBlobStats(blobArray);
	if (simCounter == 0)
	{
		recordFoodPositions(foodArray);
		pushBlobFrames();
	}
}

void simulationResults::recordSim()
{
	++simCounter;
	m_manySimAvg.push_back(m_avgBlobStats);
	m_manySimEach.push_back(m_eachBlobStats);
	m_avgBlobStats.clear();
	m_eachBlobStats.clear();

}

std::vector<std::array<double, 10>>& simulationResults::getAvgBlobStats()
{
	return m_avgBlobStats;
}

std::vector<std::vector<std::vector<double>>>& simulationResults::getEachBlobStats()
{
	return m_eachBlobStats;
}

std::vector< std::vector<std::array<double, 10>>>& simulationResults::getManySimAvg()
{
	return m_manySimAvg;
}

std::vector<std::vector<std::vector<std::vector<double>>>>& simulationResults::getManySimEach()
{
	return m_manySimEach;
}

std::vector<std::vector<Food>>& simulationResults::getEachFoodArray()
{
	return m_eachFoodArray;
}

std::vector<std::vector<std::vector<std::array<double, 5>>>>& simulationResults::getDailyBlobFrames()
{
	return m_dailyBlobframes;
}
#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
#include "blob.h"
#include "simulationResults.h"

bool sortByName(Blob &x, Blob &y)
{
	return (x.getName()) < (y.getName());
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

std::vector<Blob> simulationResults::combineBlobArrays(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray)
{
	std::vector<Blob> allBlobs{ blobArray };
	allBlobs.insert(allBlobs.end(), deadBlobArray.begin(), deadBlobArray.end());
	std::sort(allBlobs.begin(), allBlobs.end(), &sortByName);
	return allBlobs;
}

void simulationResults::recordDaysSteps(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray)
{
	//Sort Blobs into original walking order, by using name
	std::vector<Blob> allBlobs{combineBlobArrays(blobArray, deadBlobArray) };
	int length{ static_cast<int>(allBlobs.size()) };
	bool finished;
	do
	{
		finished = true;
		for (int i{ 0 }; i<length;++i)
		{
			int speed = allBlobs[i].getSpeed();
			std::vector<std::array<int,2>>& path = allBlobs[i].getPath();

			//If the blob has recorded positions in the path
			if (path.size() >= speed)
			{
				if (allBlobs[i].getName() == 12)
				{
					std::cout << "Blob #" << allBlobs[i].getName() << " has a path size of " << path.size() << "\n";
				}
				/*iterate from first position to the nth position where n is the
				number of steps that blob may take per turn. Erasing each position from
				the blobs recorded path, after copying its value onto a seperate vector*/
				int n{ 0 };
				finished = false;
				for (auto it = path.begin(); it != path.end();)
				{
					m_daysSteps.push_back(*it);
					it = (path.erase(it));
					++n;
					if (n >= speed)
					{
						it = path.end();
					}
				}
			}
			else
			{
			//	std::cout << "Path size successfully reduced to " << path.size() << "\n";
			}
		}

	} while (!finished);

	m_eachDaysSteps.push_back(m_daysSteps);
	m_daysSteps.clear();
	//erase deadBlobs as no longer needed and to prevent repeat recording of blobs
	deadBlobArray.clear();
	length = blobArray.size();
	for (int i{ 0 }; i < length; ++i)
	{
		blobArray[i].getPath().clear();
	}
}

void simulationResults::recordDay(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray)
{
	recordAvgBlobStats(blobArray);
	recordEachBlobStats(blobArray);
	recordDaysSteps(blobArray, deadBlobArray);
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

std::vector<std::array<int, 2>> simulationResults::getDaysSteps()
{
	return m_daysSteps;
}

std::vector<std::vector<std::array<int, 2>>> simulationResults::getEachDaysSteps()
{
	return m_eachDaysSteps;
}
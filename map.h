#ifndef MAP_H
#define MAP_H

#include <array>
#include <vector>
#include "blob.h"
#include "randNum.h"

class Map
{
protected:
	// Map will be a square grid for now
	int m_size;

	/* For blob animation
	std::vector<std::array<int, 2>> m_blobPositions;
	std::vector<std::vector<std::array<int, 2>>> m_blobStepTracker;
	std::vector<std::array<int, 2>> m_foodPositions;
	*/

public:
	Map();

	Map(int size);

	void setMapSize(int x);

	int getMapSize();

	std::vector<Food> populateFood(int amount);

	std::array<int, 2> getEdgePosition();

	std::vector<Blob> populateBlobs(Blob &blob, int amount);
	
	/*
	const std::vector<std::array<int, 2>>& getFoodPositions() const
	{
		return m_foodPositions;
	}

	const std::vector<std::vector<std::array<int, 2>>>& getBlobStepTracker() const
	{
		return m_blobStepTracker;
	}

	void recordBlobPositions(std::vector<Blob> &blobArray)
	{
		for (Blob blob : blobArray)
		{
			std::array<int, 2> position{ blob.getXPosition(), blob.getYPosition() };
			m_blobPositions.push_back(position);
		}
		m_blobStepTracker.push_back(m_blobPositions);
		m_blobPositions.clear();
	}

	void clearPositionData()
	{
		m_blobPositions.clear();
		m_blobStepTracker.clear();
	}

	void recordDaysMovements()
	{
		m_dailyBlobTracker.push_back(m_blobStepTracker);
	}
	*/
};

#endif
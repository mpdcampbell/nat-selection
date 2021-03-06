#include <vector>
#include <array>
#include <string>
#include <cassert>
#include "food.h"
#include "blob.h"
#include "randNum.h"
#include "map.h"

Map::Map() : m_size{ 20 }
{
}

Map::Map(int size) : m_size{ size }
{
}

void Map::setMapSize(int x)
{
	if (x >= 2)
	{
		m_size = x;
	}
	else
	{
		std::cout << "Map size must be at least 2 to create a grid for simulation";
		m_size = 2;
	}
	
}

int Map::getMapSize()
{
	return m_size;
}

std::vector<Food> Map::populateFood(int amount)
{
	std::vector<Food> foodArray;
	for (int i{ 0 }; i < amount; ++i)
	{
		int x{ getRandomNumber(1,m_size) };
		int y{ getRandomNumber(1,m_size) };
		Food food(x, y);
		foodArray.push_back(food);
	}
	return foodArray;
}

std::array<int, 2> Map::getEdgePosition()
{
	int side{ getRandomNumber(1,4) };
	std::array<int, 2> xy{ 0,0 };
	switch (side)
	{
	case 1:
	{
		xy[0] = 0;
		xy[1] = getRandomNumber(0, m_size + 1);
		break;
	}
	case 2:
	{
		xy[0] = m_size+1;
		xy[1] = getRandomNumber(0, m_size + 1);
		break;

	}
	case 3:
	{
		xy[0] = getRandomNumber(0, m_size + 1);
		xy[1] = 0;
		break;
	}
	case 4:
	{
		xy[0] = getRandomNumber(0, m_size + 1);
		xy[1] = m_size+1;
		break;
	}
	}
	return xy;
}

std::vector<Blob> Map::populateBlobs(Blob &blob, int amount)
{
	std::vector<Blob> blobArray;

	blob.setMapSize(m_size);
	for (int i{ 0 }; i < amount; ++i)
	{
		std::array<int, 2> xy{ getEdgePosition() };
		blob.setName(i + 1);
		blob.setPosition(xy[0], xy[1]);
		blob.setStepTarget(xy[0], xy[1]);
		blobArray.push_back(blob);
	}
	return blobArray;
}


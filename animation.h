#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include "blob.h"
#include "olcPixelGameEngine.h"

bool sortByName(Blob &x, Blob &y);

std::vector<Blob> combineBlobArrays(std::vector<Blob> &blobArray, std::vector<Blob> &deadBlobArray);

class BlobSim : public olc::PixelGameEngine
{
public:
	BlobSim();

private:
	int nMapWidth;
	int nMapHeight;
	int m_cellCount;
	int m_cellSize;
	int nBorderWidth;
	int cellPos = 1;
	std::vector<Blob> m_allBlobs;
	std::vector<Food> foodArray;

public:
	BlobSim(int cellCount, std::vector<Blob> &allBlobs);

	/*
	void setBlobArray(std::vector<Blob> &allBlobs)
	{
		m_allBlobs = allBlobs;
	}
	*/

	bool OnUserCreate() override;

	void playAnimation();

	bool OnUserUpdate(float fElapsedTime) override;
};

#endif
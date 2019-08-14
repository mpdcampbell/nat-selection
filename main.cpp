#include <iostream> // prints stuff to screen
#include <vector> // dynamic arrays that handle thier own memory cleanup
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#include <cassert> // for assert()

#include "gnuplot-iostream.h"
#include "food.h"
#include "blob.h"
#include "randNum.h" //random number generator
#include "map.h"
#include "simulationResults.h"
#include "graphs.h"
#include "simulation.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Animation : public olc::PixelGameEngine
{
public:
	Animation();

private:
	int m_screenCount;
	int m_homeCount;
	int m_gridCount;
	int m_cellSize;
	int m_borderWidth;
	int m_day;
	int m_frame;
	std::vector<std::vector<Food>> m_eachFoodPositions;
	std::vector<std::vector<std::vector<std::array<int, 2>>>> m_dailyBlobFrames;

public:
	Animation(int cellCount, std::vector<std::vector<Food>> &eachFoodPositions,
		std::vector<std::vector<std::vector<std::array<int,2>>>> &dailyBlobFrames);

	bool OnUserCreate() override;

	void fixCoords();

	bool OnUserUpdate(float fElapsedTime) override;
};

Animation::Animation()
{
	sAppName = "Natural Selection Simulation";
}

Animation::Animation(int cellCount, std::vector<std::vector<Food>> &eachFoodPositions,
	std::vector<std::vector<std::vector<std::array<int, 2>>>> &dailyBlobFrames)
		: m_gridCount{ cellCount }, m_eachFoodPositions{ eachFoodPositions}, 
			m_dailyBlobFrames{ dailyBlobFrames }
{

}

void Animation::fixCoords()
{
	int n{m_homeCount-1};
	for (int i{ 0 }; i< m_dailyBlobFrames.size(); ++i)
	{
		for (int j{ 0 }; j < m_dailyBlobFrames[i].size(); ++j)
		{
			for (int k{ 0 }; k < (m_dailyBlobFrames[i])[j].size(); ++k)
			{
				int x = m_dailyBlobFrames[i][j][k][0];
				int y = m_dailyBlobFrames[i][j][k][1];

				/*Rotates each point 90 degrees clockwise about origin,
				then shifts upwards by n. This converts mycartesion system
				to the matrix system used in the animation library*/
				m_dailyBlobFrames[i][j][k][0] = (y);
				m_dailyBlobFrames[i][j][k][1] = -x+n;
			}
		}
	}
}

bool Animation::OnUserCreate()
{
	m_screenCount = m_gridCount + 15;
	m_homeCount = m_gridCount + 2;
	m_cellSize = ScreenHeight() / m_screenCount;
	m_borderWidth = 1;
	m_day = 1;
	m_frame = 0;

	fixCoords();
	return true;
}


bool Animation::OnUserUpdate(float fElapsedTime)
{
	std::cout << "Day: " << m_day << "\n";
	if (GetKey(olc::Key::SPACE).bHeld)
	{
		return true;
	}

	//Clear Screen
	FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);


	//Draw Home hopefully
	olc::Pixel grey{ 192, 192, 192};
	for (int x = 1; x < m_gridCount + 3; ++x)
	{
		for (int y = 1; y < m_gridCount + 3; ++y)
		{

			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, grey);
		}
	}

	//Draw Grid / Map 
	for (int x = 2; x < m_gridCount + 2; ++x)
	{
		for (int y = 2; y < m_gridCount + 2; ++y)
		{
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize, olc::BLACK);
			FillRect(x * m_cellSize, y * m_cellSize, m_cellSize - m_borderWidth, m_cellSize - m_borderWidth, olc::WHITE);
		}
	}

	for (Food food : m_eachFoodPositions[m_day])
	{
		olc::Pixel green_50{ 0, 255, 0, 128 };
		FillRect((food.getXPosition()+1)* m_cellSize, (food.getYPosition()+1)* m_cellSize, m_cellSize - m_borderWidth, m_cellSize - m_borderWidth, green_50);
	}

	for ( auto position : (m_dailyBlobFrames[m_day])[m_frame])
	{
		olc::Pixel blue_50{ 0, 0, 255, 128 };
		FillRect((position[0]+1) * m_cellSize, (position[1]+1) * m_cellSize, m_cellSize - m_borderWidth, m_cellSize - m_borderWidth, blue_50);
		for (auto it = m_eachFoodPositions[m_day].begin(); it != m_eachFoodPositions[m_day].end();)
		{
			if (it->getXPosition() == position[0] && it->getYPosition() == position[1])
			{
				it = m_eachFoodPositions[m_day].erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	++m_frame;

	if (m_frame == static_cast<int>(m_dailyBlobFrames[m_day].size()))
	{
		++m_day;
		m_frame = 0;
	}

	if (m_day == m_dailyBlobFrames.size())
	{
		return false;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	return true;
}


int main()
{
	srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock
	rand(); // remove first value

	Map map;
	std::vector<Blob> blobArray;
	std::vector<Blob> deadBlobArray;
	std::vector<Food> foodArray;
	simulationResults stats;
	
	//INITIAL BLOB STATS
	double nativeEnergy{ 2000.0 };
	double seedSize{ 3.0 }; 
	double seedSpeed{ 3.0 };
	double seedSense{ 6.0 }; 
	Blob seedBlob{ nativeEnergy, seedSize, seedSpeed, seedSense };

	//ENVIRONMENT VARIABLES
	map.setMapSize(30); // integer length, in grid spaces, of one side of the square map
	int seedBlobCount{ 30 }; //starting number of Blobs
	int foodCount{ 50 }; // number of food pieces place randomly on map daily

	//SIMULATION VARIABLES
	g_mutationProb = 20; //integer probability of a blob stat mutating during replication
	int dayCount{200}; // length of simulation in days
	int simCount{1}; // number of repeat simulations run

	for (int sim{ 0 }; sim < simCount; ++sim)
	{
		g_nameHolder = seedBlobCount+1; //For specific blob naming, allows error tracking
		blobArray = map.populateBlobs(seedBlob, seedBlobCount);
		foodArray = map.populateFood(foodCount);

		for (int day{ 0 }; day < dayCount; ++day)
		{
			std::cout << "Run #" << sim << ", Day #" << day << "\n";
			//a check to end early incase of extinction
			if (blobArray.size() == 0)
			{
				std::cout << "Extinction on day " << day << "\n";
				break;
			}
			stats.recordDay(blobArray, deadBlobArray, foodArray);
			walkAndEat(blobArray, deadBlobArray, foodArray, stats);
			naturalSelection(blobArray, deadBlobArray);
			breed(blobArray, day);
			digestAndSleep(blobArray);
			foodArray = map.populateFood(foodCount);
		}
		stats.recordSim();
	}
	
	//GRAPHS OUTPUT
	makeAvgGraphs(stats); //line graph of population and mean size, speed and sense each day 
	
	int length = map.getMapSize();
	std::vector<std::vector<Food>> eachFoodArray = stats.getEachFoodArray();
	auto dailyBlobFrames = stats.getDailyBlobFrames();
	
	/*
	for (auto day : dailyBlobFrames)
	{
		for (auto frameArray : day)
		{
			for (auto frame : frameArray)
			{
				if (frame[0] > (length + 1) || frame[1] > (length + 1))
				{
					std::cout << frame[0] << ", " << frame[1] << "\n";
				}
			}
		}
	}
	*/

	
	Animation blobSim(length, eachFoodArray, dailyBlobFrames);

	if (blobSim.Construct(720, 720, 1, 1))
	{
		blobSim.Start();
	}
	
	//int firstSim{ 0 }, lastSim{ 0 }; //Which simulation runs to create histogram gifs for
	
	//makeHistogram(stats, firstSim, lastSim); //Creates gif of daily size, speed and sense distribution

	system("pause");
	
	return 0;
}
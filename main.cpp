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
#include "olcConsoleGameEngine.h"

class Animation : public olcConsoleGameEngine
{
public:
	Animation();

private:
	int m_cellCount;
	int m_day;
	int m_frame;
	std::vector<std::vector<Food>> m_eachFoodPositions;
	std::vector<std::vector<std::vector<std::array<int, 2>>>> m_dailyBlobFrames;

public:
	Animation(int cellCount, std::vector<std::vector<Food>> &eachFoodPositions,
		std::vector<std::vector<std::vector<std::array<int,2>>>> &dailyBlobFrames);

	bool OnUserCreate() override;

	//bool playAnimation();

	bool OnUserUpdate(float fElapsedTime) override;
};

Animation::Animation()
{
	m_sAppName = L"Natural Selection Simulation";
}

Animation::Animation(int cellCount, std::vector<std::vector<Food>> &eachFoodPositions,
	std::vector<std::vector<std::vector<std::array<int, 2>>>> &dailyBlobFrames)
	: m_cellCount{ cellCount }, m_eachFoodPositions{ eachFoodPositions}, 
		m_dailyBlobFrames{ dailyBlobFrames }
{

}

bool Animation::OnUserCreate()
{
	m_day = 1;
	m_frame = 0;
	return true;
}


bool Animation::OnUserUpdate(float fElapsedTime)
{

	if (m_keys[VK_SPACE].bHeld)
	{
		return true;
	}

	//Draw grid hopefully

	for (int x = 0; x < m_cellCount; x++)
	{
		for (int y = 0; y < m_cellCount; y++)
		{
			//olc::Pixel colour = olc::BLUE;

			// Draw Base
			Draw(x, y, PIXEL_SOLID, FG_WHITE);
		}
	}

	for (Food food : m_eachFoodPositions[m_day])
	{
		Draw(food.getXPosition(), food.getYPosition(), PIXEL_SOLID, FG_DARK_GREEN);
	}

	for ( auto position : (m_dailyBlobFrames[m_day])[m_frame])
	{
		Draw(position[0], position[1], PIXEL_SOLID, FG_DARK_BLUE);
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

		for (int x = 0; x < m_cellCount; x++)
		{
			for (int y = 0; y < m_cellCount; y++)
			{
				//olc::Pixel colour = olc::BLUE;

				// Draw Base
				Draw(x, y, PIXEL_SOLID, FG_RED);
			}
		}
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
	double seedSense{ 3.0 }; 
	Blob seedBlob{ nativeEnergy, seedSize, seedSpeed, seedSense };

	//ENVIRONMENT VARIABLES
	map.setMapSize(30); // integer length, in grid spaces, of one side of the square map
	int seedBlobCount{ 45 }; //starting number of Blobs
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
	Animation blobSim(length, eachFoodArray, dailyBlobFrames);

	if (blobSim.ConstructConsole(map.getMapSize(), map.getMapSize(), 16, 16))
	{
		blobSim.Start();
	}

	//int firstSim{ 0 }, lastSim{ 0 }; //Which simulation runs to create histogram gifs for
	
	//makeHistogram(stats, firstSim, lastSim); //Creates gif of daily size, speed and sense distribution
	
	system("pause");
	return 0;
}
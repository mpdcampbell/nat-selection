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
#include "animation.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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
	int seedBlobCount{ 40 }; //starting number of Blobs
	int foodCount{ 80 }; // number of food pieces place randomly on map daily

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
			stats.recordDay(blobArray, deadBlobArray);
			walkAndEat(blobArray, deadBlobArray, foodArray);
			naturalSelection(blobArray, deadBlobArray);
			breed(blobArray, day);
			digestAndSleep(blobArray);
			foodArray = map.populateFood(foodCount);
		}
		stats.recordSim();
	}
	
	//GRAPHS OUTPUT
	makeAvgGraphs(stats); //line graph of population and mean size, speed and sense each day 

	std::cout << "Day Count: " << dayCount << ", Days recorded in vector: " << stats.getEachDaysSteps().size() << "\n";

	/*
	BlobSim demo{ map.getMapSize(), allBlobs };
	uint32_t screenHeight{ 612 };
	uint32_t screenWidth{ screenHeight };

	if (demo.Construct(screenWidth, screenHeight, 1, 1))
	{
		demo.Start();
	}


	int firstSim{ 0 }, lastSim{ 0 }; //Which simulation runs to create histogram gifs for
	
	//makeHistogram(stats, firstSim, lastSim); //Creates gif of daily size, speed and sense distribution
	*/
	system("pause");
	return 0;
}
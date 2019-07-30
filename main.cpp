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

int main()
{
	srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock
	rand(); // remove first value

	Map map;
	std::vector<Blob> blobArray;
	std::vector<Food> foodArray;
	simulationResults stats;
	
	//INITIAL BLOB STATS
	double nativeEnergy{ 2000.0 };
	double seedSize{ 3.0 }; 
	double seedSpeed{ 3.0 };
	double seedSense{ 3.0 }; 
	Blob seedBlob{ nativeEnergy, seedSize, seedSpeed, seedSense };

	//SIMULATION VARIABLES
	g_mutationProb = 20; //Int Probability of a blob stat mutating during replication
	map.setMapSize(30); // Int length, in grid spaces, of one side of the square map
	int seedBlobCount{ 40 }; //starting number of Blobs
	int foodCount{80}; // number of food pieces place randomly on map daily
	int dayCount{1000}; // length of simulation in days
	int simCount{6}; // number of repeat simulations run

	for (int sim{ 0 }; sim < simCount; ++sim)
	{
		g_nameHolder = seedBlobCount; //For specific blob naming, allows error tracking
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
			stats.recordDay(blobArray);
			blobsCarryOutDay(blobArray, foodArray);
			foodArray = map.populateFood(foodCount);
		}
		stats.recordSim();
	}
	
	//GRAPHS OUTPUT
	makeAvgGraphs(stats); //line graph of population and mean size, speed and sense each day 
	
	int firstSim{ 0 }, lastSim{ 0 }; //Which simulation runs to create histogram gifs for
	
	//makeHistogram(stats, firstSim, lastSim); //Creates gif of daily size, speed and sense distribution

	return 0;
}
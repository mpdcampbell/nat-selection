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


int main()
{
	srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock
	rand(); // remove first value

	Map map;
	std::vector<Blob> blobArray;
	std::vector<Food> foodArray;
	simulationResults stats;

	//INITIAL BLOB STATS
	double nativeEnergy{ 600.0 };
	double seedSize{ 0.5 };
	double seedSpeed{ 2.0 };
	double seedSense{ 1.0 };
	Blob seedBlob( nativeEnergy, seedSize, seedSpeed, seedSense);

	//ENVIRONMENT VARIABLES
	map.setMapSize( 8 ); //Integer length, in grid spaces, of one side of the square map
	int seedBlobCount{ 12 }; //Starting number of Blobs
	int foodCount{ 10 }; //Number of food pieces place randomly on map daily

	//SIMULATION VARIABLES
	g_mutationProb = 40; //Integer probability (%) of a blob stat mutating during replication
	int dayCount{ 50 }; //Length of simulation in days
	int simCount{ 1 }; //Number of repeat simulations run

	//GRAPH VARIABLES
	int firstSim{ 0 }, lastSim{ 0 }; //Range of simulation runs to create histogram gifs for

	//ANIMATION VARIABLES
	int yResolution{ 650 }; //Animation window resolution in pixels
	int xResolution{ 950 };
	Animation::ColourStat colourStat{ Animation::SIZE }; // SIZE, SPEED or SENSE
	std::string vidName{  }; //Video filename, if blank default of xM_xB_xF_xD_Stat is used

	for (int sim{ 0 }; sim < simCount; ++sim)
	{
		g_nameHolder = seedBlobCount + 1; //For specific blob naming, allows error tracking
		blobArray = map.populateBlobs(seedBlob, seedBlobCount);
		foodArray = map.populateFood(foodCount);
				
		for (int day{ 0 }; day < dayCount; ++day)
		{
			stats.recordDay(blobArray, foodArray);
			std::cout << "Run #" << sim << ", Day #" << day << "\n";

			walkAndEat(blobArray, foodArray, stats);
			stats.pushBlobFrames();
			naturalSelection(blobArray);
			//a check to end early incase of extinction
			if (blobArray.size() == 0)
			{
				std::cout << "Extinction at end of day " << day << "\n";
				break;
			}
			breed(blobArray);
			digestAndSleep(blobArray);
			foodArray = map.populateFood(foodCount);

		}
		stats.recordSim();
	}

	//GRAPHS OUTPUT
	makeAvgGraphs(stats); //line graph of population and mean size, speed and sense each day
	//makeHistogram(stats, firstSim, lastSim); //Creates gif of daily size, speed and sense distribution

	//ANIMATION
	Animation blobSim(map.getMapSize(), stats, vidName, colourStat); //Creates animation object
	if (blobSim.Construct(xResolution, yResolution, 1, 1)) //Runs animation
	{
		blobSim.Start();
	}

	return 0;
}
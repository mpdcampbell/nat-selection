#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#include "randNum.h"

int getRandomNumber(int min, int max)
{
	// static used for efficiency, so we only calculate this value once
	static const double fraction = 1.0 / ((static_cast<double>(RAND_MAX)) + 1.0);
	// evenly distribute the random number across our range																			 
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
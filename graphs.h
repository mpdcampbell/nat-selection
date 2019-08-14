#ifndef GRAPHS_H
#define GRAPHS_H

#include <vector>
#include "simulationResults.h"

void makeAvgGraphs(simulationResults &stats);

void makeHistogram(simulationResults &stats, int firstSim, int lastSim);

#endif
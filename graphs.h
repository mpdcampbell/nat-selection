#ifndef GRAPHS_H
#define GRAPHS_H

#include <vector>
#include "simulationResults.h"

double getMax(std::vector<double> &vector);

void makeAvgGraphs(simulationResults &stats);

void makeHistogram(simulationResults &stats, int firstSim, int lastSim);

#endif
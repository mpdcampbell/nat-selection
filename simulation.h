#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include "food.h"
#include "blob.h"
#include "simulationResults.h"

extern int g_nameHolder;

void walkAndEat(std::vector<Blob> &blobArray, std::vector<Food> &foodArray, simulationResults &stats);

void naturalSelection(std::vector<Blob> &blobArray);

void breed(std::vector<Blob> &blobArray);

void digestAndSleep(std::vector<Blob> &blobArray);

#endif
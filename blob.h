#ifndef BLOB_H
#define BLOB_H

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <array>
#include "thing.h"
#include "food.h"

extern int g_mutationProb;

class Blob : public Thing
{
protected:
	double m_nativeEnergy;
	double m_energy = m_nativeEnergy;
	double m_sense;
	double m_speed;
	double m_size;
	int m_foodEaten = 0;
	int m_mapSize;
	int m_name;
	std::array<double, 2> m_stepTarget;

public:

	Blob();

	Blob(double nativeEnergy, double size, double speed, double sense);

	int getName();

	void setName(int name);

	double getSense();

	void setSense(double x);

	double getSpeed();

	void setSpeed(double x);

	double getSize();

	void setSize(double x);

	double getCost();

	double getEnergy();

	void reduceEnergy();

	void setEnergy(double x);

	int getMapSize();

	void setMapSize(int x);

	int getFoodEaten();

	void setFoodEaten(int x);

	void sleep();

	void stepNorth();

	void stepSouth();

	void stepEast();

	void stepWest();

	void stepEastOrWest();

	void stepNorthOrSouth();

	void randomStep();

	void stepTowards(Thing &thing);

	void stepAway(Thing &thing);

	double distToObject(Thing &object);

	bool atHome();

	void goHome();

	void searchPattern();

	double distToEdge();

	bool hasSurplusStamina();

	void huntOrRun(std::vector<Blob> &blobArray, std::vector<Food> &foodArray);
	
	std::optional<int> lookForFood(std::vector<Food> &foodArray);

	std::optional<int> lookForPrey(std::vector<Blob> &blobArray);

	std::optional<int> lookForPredator(std::vector<Blob> &blobArray);

	std::optional<int> tryToEat(std::vector<Blob> &blobArray, std::vector<Food> &foodArray);

	void mutate();

	std::optional<Blob> tryToReplicate();

	void setStepTarget(double x, double y);

	std::array<double, 2>& getStepTarget();

	void continueStep();

	bool finishedStep();
};

#endif

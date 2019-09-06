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
	int m_birthday;

public:

	Blob();

	Blob(double nativeEnergy, double size, double speed, double sense);

	int getName();

	void setName(int name);

	double getSense();

	void setSense(double x);

	double getSpeed();

	void setSpeed(double x);

	double getCost();

	double getEnergy();

	void reduceEnergy();

	void setEnergy(double x);

	double getSize();

	void setSize(double x);

	void setMapSize(int x);

	int getMapSize();

	int getFoodEaten();

	void setFoodEaten(int x);

	void sleep();

	void stepNorth();

	void stepSouth();

	void stepEast();

	void stepWest();

	void randomStep();

	void stepTowards(Thing &thing);

	void stepAway(Thing &thing);

	bool atFood(Food &food);

	std::optional<int> huntOrRun(std::vector<Blob> &blobArray, std::vector<Food> &foodArray);
	
	int distToObject(Thing &object);

	std::optional<int> lookForFood(std::vector<Food> &foodArray);

	std::optional<int> lookForPrey(std::vector<Blob> &blobArray);

	std::optional<int> lookForPredator(std::vector<Blob> &blobArray);

	bool atHome();

	void goHome();

	void headInland();

	void blindSearch();

	int distToEdge();

	bool hasSurplusStamina();

	void mutate();

	std::optional<Blob> tryToReplicate();
};

#endif

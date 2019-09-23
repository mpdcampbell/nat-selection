#include <string>
#include <optional>
#include <vector>
#include "thing.h"
#include "food.h"
#include "randNum.h"
#include "blob.h"

extern int g_mutationProb{ 10 };

Blob::Blob()
	: m_nativeEnergy{ 1800.0 }, m_size{ 3.0 }, m_speed{ 3.0 }, m_sense{ 3.0 }, m_mapSize{ 8 }
{
}

Blob::Blob(double nativeEnergy, double size, double speed, double sense)
	: m_nativeEnergy{ nativeEnergy }, m_size{ size }, m_speed{ speed }, m_sense{ sense }, m_mapSize{ 8 }
{
}

int Blob::getName()
{
	return m_name;
}

void Blob::setName(int name)
{
	m_name = name;
}

double Blob::getSense()
{
	return m_sense;
}

void Blob::setSense(double x)
{
	m_sense = x;
}

double Blob::getSpeed()
{
	return m_speed;
}

void Blob::setSpeed(double x)
{
	m_speed = x;
}

double Blob::getCost()
{
	double cost = (m_size*m_size*m_size) + (m_speed*m_speed) + m_sense;
	return cost;
}

double Blob::getEnergy()
{
	return m_energy;
}

void Blob::reduceEnergy()
{
	m_energy -= getCost();
}

void Blob::setEnergy(double x)
{
	m_energy = x;
}

double Blob::getSize()
{
	return m_size;
}

void Blob::setSize(double x)
{
	m_size = x;
}

void Blob::setMapSize(int x)
{
	m_mapSize = x;
}

int Blob::getMapSize()
{
	return m_mapSize;
}

int Blob::getFoodEaten()
{
	return m_foodEaten;
}

void Blob::setFoodEaten(int x)
{
	m_foodEaten = x;
}

void Blob::sleep()
{
	m_energy = m_nativeEnergy;
}

/*Using convention that North and East are +ve and 
South and West are -ve (cartesian)*/
void Blob::stepNorth()
{
	double y{ m_yPosition + 1.0 };
	setStepTarget(m_xPosition, y);
}

void Blob::stepSouth()
{
	double y{ m_yPosition - 1.0 };
	setStepTarget(m_xPosition, y);
}

void Blob::stepEast()
{
	double x{ m_xPosition + 1.0 };
	setStepTarget(x, m_yPosition);
}

void Blob::stepWest()
{
	double x{ m_xPosition - 1.0 };
	setStepTarget(x, m_yPosition);
}

void Blob::randomStep()
{
	/*create an array of function pointers, 
	specifically pointers to functions inside Blob class, 
	that take no inputs and return void*/
	std::vector<void(Blob::*)()> compass;
	double d_mapSize{ static_cast<double>(m_mapSize) };
	// if the blob isn't against the east boundary
	if (m_xPosition < d_mapSize-1.0)
	{
		//add a function pointer to the array
		//pointing to address of stepEast
		compass.push_back(&Blob::stepEast);
	}
	// if the blob isn't against the West boundary
	 if (m_xPosition >= 2.0)
	{
		compass.push_back(&Blob::stepWest);
	}
	// if the blob isn't against the North boundary
	if (m_yPosition < d_mapSize-1.0)
	{
		compass.push_back(&Blob::stepNorth);
	}
	// if the blob isn't against the South boundary
	 if (m_yPosition >= 2.0)
	{
		compass.push_back(&Blob::stepSouth);
	}
	//Randomly choose direction and take one step
	int num{ getRandomNumber(1, (static_cast<int>(compass.size()))) };
	(this->*compass[num - 1])();
}

void Blob::stepEastOrWest()
{
	std::vector<void(Blob::*)()> compass;
	double d_mapSize{ static_cast<double>(m_mapSize) };
	if (m_xPosition < d_mapSize-1.0)
	{
		compass.push_back(&Blob::stepEast);
	}
	if (m_xPosition >= 2.0)
	{
		compass.push_back(&Blob::stepWest);
	}
	int num{ getRandomNumber(1, (static_cast<int>(compass.size()))) };
	(this->*compass[num - 1])();
}

void Blob::stepNorthOrSouth()
{
	std::vector<void(Blob::*)()> compass;
	double d_mapSize{ static_cast<double>(m_mapSize) };
	if (m_yPosition < d_mapSize-1.0)
	{
		compass.push_back(&Blob::stepNorth);
	}
	if (m_yPosition >= 2.0)
	{
		compass.push_back(&Blob::stepSouth);
	}
	int num{ getRandomNumber(1, (static_cast<int>(compass.size()))) };
	(this->*compass[num - 1])();
}

void Blob::stepTowards(Thing &thing)
{
	double xdif = thing.getXPosition() - m_xPosition;
	double ydif = thing.getYPosition() - m_yPosition;
	double xTarget{ m_xPosition };
	double yTarget{ m_yPosition };
	
	(std::abs(xdif) > std::abs(ydif)) ?
		(xTarget += (xdif / std::abs(xdif))) : 
		(yTarget += (ydif / std::abs(ydif)));

	setStepTarget(xTarget, yTarget);
}

void Blob::stepAway(Thing &thing)
{
	double xdif = thing.getXPosition() - m_xPosition;
	double ydif = thing.getYPosition() - m_yPosition;

	double xTarget{ m_xPosition };
	double yTarget{ m_yPosition };
	/* Blobs at Home that want to run from predators, 
	can run but only within the boundaries.*/

	if (m_xPosition < 1.0 || m_xPosition > m_mapSize)
	{
		if (m_yPosition < 1.0 || m_yPosition > m_mapSize)
		{
			//if in corner of map area
			randomStep();
		}
		else
		{
			/*If ydiff = 0 then Thing is on same row, so step North 
			or South is equally good. Otherwise increase distance to 
			pred, staying within column*/
			(ydif == 0.0) ? (stepNorthOrSouth()) :
				(yTarget -= ydif / std::abs(ydif));
		}
	}
	else if (m_yPosition < 1.0 || m_yPosition > m_mapSize)
	{
			/*If xdiff = 0 then Thing is on same column, so step West
			or East is equally good. Otherwise increase distance to 
			pred, staying within row*/
		(xdif == 0) ? (stepEastOrWest()) :
			(xTarget -= xdif / std::abs(xdif));
	}

	else
	{
		if (ydif == 0.0 && xdif == 0.0)
		{
			randomStep();
		}
		else
		{
			/*Blobs want to run from predator in general, movement not as restricted*/
			(std::abs(xdif) > std::abs(ydif)) ?
				(xTarget -= xdif / std::abs(xdif)) :
				(yTarget -= ydif / std::abs(ydif));
		}
	}
	if (xTarget != m_xPosition || yTarget != m_yPosition)
	{
		setStepTarget(xTarget, yTarget);
	}
}

double Blob::distToObject(Thing &object)
{
	double xdif = object.getXPosition() - m_xPosition;
	double ydif = object.getYPosition() - m_yPosition;
	double distAway{ std::abs(xdif) + std::abs(ydif) };
	return distAway;
}

void Blob::huntOrRun(std::vector<Blob> &blobArray, std::vector<Food> &foodArray)
{
	std::optional<int> predOpt = lookForPredator(blobArray);
	std::optional<int> foodOpt = lookForFood(foodArray);
	std::optional<int> preyOpt = lookForPrey(blobArray);
	//guaranteed larger than any dist to object on map
	double foodDist( m_mapSize * 10.0 );
	double preyDist( m_mapSize * 10.0 );
	double predDist( m_mapSize * 10.0 );

	if (foodOpt.has_value())
	{
		int foodElement{ foodOpt.value() };
		foodDist = distToObject(foodArray[foodElement]);
	}
	if (preyOpt.has_value())
	{
		int preyElement{ preyOpt.value() };
		preyDist = distToObject(blobArray[preyElement]);
	}
	if (predOpt.has_value())
	{
		int predElement{ predOpt.value() };
		predDist = distToObject(blobArray[predElement]);
	}
	// if none of the above have value take random step
	else if (!foodOpt.has_value() && !preyOpt.has_value())
	{
		searchPattern();
		return;
	}

	//catch start ontop of food/prey error
	if (foodDist == 0.0 || preyDist == 0.0)
	{
		setStepTarget(m_xPosition, m_yPosition);
		return;
	}

	if (predDist < ((foodDist < preyDist) ? foodDist : preyDist))
	{
		stepAway(blobArray[predOpt.value()]);
		return;
	}

	if (predDist > ((foodDist < preyDist) ? foodDist : preyDist))
	{
		(foodDist < preyDist) ?
			stepTowards(foodArray[foodOpt.value()]) :
			stepTowards(blobArray[preyOpt.value()]);
		return;
	}

	if (predDist == ((foodDist < preyDist) ? foodDist : preyDist))
	{
		if (getFoodEaten() == 0)
		{
			(foodDist < preyDist) ?
				stepTowards(foodArray[foodOpt.value()]) :
				stepTowards(blobArray[preyOpt.value()]);
		}
		else
		{
			stepAway(blobArray[predOpt.value()]);
		}
		return;
	}
	std::cout << "\nShould be impossible to reach here\n";
	return;
}

std::optional<int> Blob::tryToEat(std::vector<Blob> &blobArray, std::vector<Food> &foodArray)
{
	std::optional<int> foodOpt = lookForFood(foodArray);
	std::optional<int> preyOpt = lookForPrey(blobArray);

	if (foodOpt.has_value())
	{
		int foodElement{ foodOpt.value() };
		double foodDist = distToObject(foodArray[foodElement]);
		if (foodDist == 0.0) //if ontop of food
		{
			//erase that food element
			auto it = foodArray.begin();
			foodArray.erase(it + foodOpt.value());
			setFoodEaten(getFoodEaten() + 1);
			return std::nullopt;
		}
	}
	if (preyOpt.has_value())
	{
		int preyElement{ preyOpt.value() };
		double preyDist = distToObject(blobArray[preyElement]);
		if (preyDist == 0.0) //if ontop of prey
		{
			auto it = blobArray.begin();
			//erase that blob element
			blobArray.erase(it + preyOpt.value());
			setFoodEaten(getFoodEaten() + 1);
			return preyOpt.value();
		}
	}

	//If neither dist are 0.0
	return std::nullopt;
}

std::optional<int> Blob::lookForFood(std::vector<Food> &foodArray)
{
	double dist = 10 * m_mapSize;
	int element;
	bool foundFood = false;

	int length{ static_cast<int>(foodArray.size()) };
	for (int i = 0; i < length; ++i)
	{
		double xPos{ foodArray[i].getXPosition() };
		double yPos{ foodArray[i].getYPosition() };
		if ((m_xPosition - m_sense) <= xPos && xPos <= (m_xPosition + m_sense))
		{
			if ((m_yPosition - m_sense) <= yPos && yPos <= (m_yPosition + m_sense))
			{
				if (distToObject(foodArray[i]) < dist)
				{
					foundFood = true;
					dist = distToObject(foodArray[i]);
					element = i;
				}
			}
		}
	}
	if (foundFood)
	{
		return element;
	}
	else
	{
		return std::nullopt;
	}
}

std::optional<int> Blob::lookForPrey(std::vector<Blob> &blobArray)
{
	double dist = 10 * m_mapSize;
	int element;
	bool foundPrey = false;

	int length{ static_cast<int>(blobArray.size()) };
	for (int i = 0; i < length; ++i)
	{
		/*is blob is 80% or less of my size and isn't at home then it is legitimate prey.
		See if it falls within sense radius*/
		if (blobArray[i].getSize() <= (0.8*m_size) && !(blobArray[i].atHome()))
		{
			double xPos{ blobArray[i].getXPosition() };
			double yPos{ blobArray[i].getYPosition() };
			if ((m_xPosition - m_sense) <= xPos && xPos <= (m_xPosition + m_sense))
			{
				if ((m_yPosition - m_sense) <= yPos && yPos <= (m_yPosition + m_sense))
				{
					if (distToObject(blobArray[i]) < dist)
					{
						foundPrey = true;
						dist = distToObject(blobArray[i]);
						element = i;
					}
				}
			}
		}
	}

	if (foundPrey)
	{
		return element;
	}
	else
	{
		return std::nullopt;
	}
}

std::optional<int> Blob::lookForPredator(std::vector<Blob> &blobArray)
{
	double dist = 10 * m_mapSize;
	int element;
	bool foundPred = false;

	int length{ static_cast<int>(blobArray.size()) };
	for (int i = 0; i < length; ++i)
	{
		if ((0.8*blobArray[i].getSize()) >= m_size && !blobArray[i].atHome())
		{
			if (blobArray[i].getEnergy() >= blobArray[i].getCost())
			{
				double xPos{ blobArray[i].getXPosition() };
				double yPos{ blobArray[i].getYPosition() };
				if ((m_xPosition - m_sense) <= xPos && xPos <= (m_xPosition + m_sense))
				{
					if ((m_yPosition - m_sense) <= yPos && yPos <= (m_yPosition + m_sense))
					{
						if (distToObject(blobArray[i]) < dist)
						{
							foundPred = true;
							dist = distToObject(blobArray[i]);
							element = i;
						}
					}
				}
			}
		}
	}
	if (foundPred)
	{
		return element;
	}
	else
	{
		return std::nullopt;
	}
}

bool Blob::atHome()
{
	if (m_xPosition < 1.0 || m_xPosition > (m_mapSize) )
	{
		return true;
	}
	if (m_yPosition < 1.0 || m_yPosition > (m_mapSize) )
	{
		return true;
	}
	
	return false;
}

void Blob::goHome()
{
	if (atHome())
	{
		m_energy = 0.0;
		return;
	}
	else
	{
		double xdif = m_xPosition - (m_mapSize / 2.0);
		double ydif = m_yPosition - (m_mapSize / 2.0);
		double xTarget{ m_xPosition };
		double yTarget{ m_yPosition };
		if (xdif == ydif) //centre diagonal
		{
			(yTarget >= m_mapSize / 2.0) ?
				(++yTarget) : (--yTarget);
		}
		else
		{
			(std::abs(xdif) > std::abs(ydif)) ?
				(xTarget += xdif / std::abs(xdif)) : 
				(yTarget += ydif / std::abs(ydif));
		}
		setStepTarget(xTarget, yTarget);
	}
}

void Blob::searchPattern()
{
	if (atHome())
	{
		randomStep();
		return;
	}

	int x{ static_cast<int>(m_xPosition) };
	if (x % 2 == 0)
	{
		if (m_yPosition <= 1.0 + m_sense)
		{
			if (m_xPosition <= 1.0 + m_sense)
			{
				stepEast();
			}
			else if (m_xPosition >= static_cast<double>(m_mapSize) - m_sense)
			{
				stepWest();
			}
			else
			{
				stepEastOrWest();
			}
		} 
		else
		{
			stepSouth();
		}
	}
	else
	{
		if (m_yPosition >= static_cast<double>(m_mapSize) - m_sense)
		{
			if (m_xPosition <= 1.0 + m_sense)
			{
				stepEast();
			}
			else if (m_xPosition >= static_cast<double>(m_mapSize) - m_sense)
			{
				stepWest();
			}
			else
			{
				stepEastOrWest();
			}
		}
		else
		{
			stepNorth();
		}
	}
}

double Blob::distToEdge()
{
	double mid( (m_mapSize + 1.0) / 2.0 );
	double xSteps, ySteps;
	double dist;
	xSteps = mid - std::abs(m_xPosition - mid);
	ySteps = mid - std::abs(m_yPosition - mid);
	(xSteps >= ySteps) ? dist = ySteps : dist = xSteps;
	return dist;
}

bool Blob::hasSurplusStamina()
{
	if (atHome())
	{
		//stop blob movement after returning home with 1 food
		return false; 
	}

	double dist{ distToEdge() };
	/*if remaining stamina is greater than that needed to get to
	closest edge then blob has spare stamina. The +1 is for case
	where blob has spare stamina then steps	one away, reducing its
	stamina by one and increasing the needed stamina by one.
	This effective +2 means without +1 error margin blob would
	make the wrong choice, and be unable to get home
	*/
	if ((m_energy / getCost()) <= dist + 1.0)
	{
		return false;
	}
	return true;
}

void Blob::mutate()
{
	int prob{ 200/ g_mutationProb};
	int num{ getRandomNumber(1, prob) };
	int max{ 10 };
	if (num == 1) 
	{
		int mult = getRandomNumber(1, 10);
		double x{ m_speed + (mult / 10.0) };
		setSpeed(x);
	}
	else if (num == 2)
	{
		if (m_speed < 1.0)
		{
			max = static_cast<int>(m_speed * 10.0);
		}
		int mult = getRandomNumber(1, max);
		double x{ m_speed - (mult / 10.0) };
		setSpeed(x);
	}
	num = getRandomNumber(1, prob);
	if (num == 1) 
	{
		int mult = getRandomNumber(1, 10);
		double x{ m_sense + (mult / 10.0) };
		setSense(x);
	}
	else if (num == 2)
	{
		max = 10;
		if (m_sense < 1.0)
		{
			max = static_cast<int>(m_sense * 10.0);
		}
		int mult = getRandomNumber(1, max);
		double x{ m_sense - (mult / 10.0) };
		setSense(x);
	}
	num = getRandomNumber(1, prob);
	if (num == 1) 
	{
		int mult = getRandomNumber(1, 10);
		double x{ m_size + (mult / 10.0) };
		setSize(x);
	}
	else if (num == 2)
	{
		max = 10;
		if (m_size < 1.0)
		{
			max = static_cast<int>(m_size * 10.0);
		}
		int mult = getRandomNumber(1, max);
		double x{ m_size - (mult / 10.0) };
		setSize(x);

	}
}

std::optional<Blob> Blob::tryToReplicate()
{
	if (getFoodEaten() > 1)
	{
		setFoodEaten(0);
		Blob child = *this;
		child.mutate();
		return child;
	}
	else
	{
		return std::nullopt;
	}
}

void Blob::setStepTarget(double x, double y)
{
	m_stepTarget = { x, y };
}

std::array<double, 2>& Blob::getStepTarget()
{
	return m_stepTarget;
}

bool Blob::finishedStep()
{
	double xdiff = m_stepTarget[0] - m_xPosition;
	double ydiff = m_stepTarget[1] - m_yPosition;
	if (xdiff == 0.0 && ydiff == 0.0)
	{
		return true;
	}

	return false;
}

void Blob::continueStep()
{
	double xdiff = m_stepTarget[0] - m_xPosition;
	double ydiff = m_stepTarget[1] - m_yPosition;
	double speedMax = 20.0;
	double babySteps{ speedMax / m_speed };
	double distPerTime = (1.0 / babySteps);

	if (xdiff != 0)
	{
		if (xdiff > 0)
		{
			m_xPosition += distPerTime;
			if (m_xPosition >= m_stepTarget[0])
			{
				setPosition(m_stepTarget[0], m_stepTarget[1]);
				reduceEnergy();
			}
		}
		if (xdiff < 0)
		{
			m_xPosition -= distPerTime;
			if (m_xPosition <= m_stepTarget[0])
			{
				setPosition(m_stepTarget[0], m_stepTarget[1]);
				reduceEnergy();
			}
		}
	}

	if (ydiff != 0)
	{
		if (ydiff > 0)
		{
			m_yPosition += distPerTime;
			if (m_yPosition >= m_stepTarget[1])
			{
				setPosition(m_stepTarget[0], m_stepTarget[1]);
				reduceEnergy();
			}
		}
		if (ydiff < 0)
		{
			m_yPosition -= distPerTime;
			if (m_yPosition <= m_stepTarget[1])
			{
				setPosition(m_stepTarget[0], m_stepTarget[1]);
				reduceEnergy();
			}
		}
	}
}
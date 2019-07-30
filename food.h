#ifndef FOOD_H
#define FOOD_H

#include<string>
#include"thing.h"

class Food : public Thing
{
protected:

public:
	//Probably a better inheritance way to handle this
	Food(int x, int y);

};

#endif
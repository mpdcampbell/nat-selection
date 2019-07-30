#ifndef THING_H
#define THING_H

#include <iostream>
#include <string>


class Thing //Need better name for parent class
{
protected:
	int m_xPosition;
	int m_yPosition;
	std::string m_name; // Need better way to handle this

public:
	Thing();

	Thing(int x, int y, std::string name);

	int getXPosition();


	int getYPosition();


	void setPosition(int x, int y);


	std::string getName();


	void printPosition();

};

#endif
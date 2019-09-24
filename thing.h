#ifndef THING_H
#define THING_H

#include <iostream>
#include <string>


class Thing //Need better name for parent class
{
protected:
	double m_xPosition;
	double m_yPosition;
	std::string m_name; // Need better way to handle this

public:
	Thing();

	Thing(double x, double y, std::string name);

	double getXPosition();


	double getYPosition();


	void setPosition(double x, double y);


	std::string getName();


	void printPosition();

};

#endif
#include <iostream>
#include <string>
#include "thing.h"

Thing::Thing() : m_xPosition{ 0 }, m_yPosition{ 0 }, m_name{ "Thing" }
{
}

Thing::Thing(int x, int y, std::string name)
	: m_xPosition{ x }, m_yPosition{ y }, m_name{ name }
{
}

int Thing::getXPosition()
{
	return m_xPosition;
}

int Thing::getYPosition()
{
	return m_yPosition;
}

void Thing::setPosition(int x, int y)
{
	m_xPosition = x;
	m_yPosition = y;
}

std::string Thing::getName()
{
	return m_name;
}

void Thing::printPosition()
{
	std::cout << m_name << " is at position " 
		<< m_xPosition << ", " << m_yPosition << ".\n";
}


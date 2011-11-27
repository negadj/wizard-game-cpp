/*
 * Triplet.cpp
 *
 *  Created on: 9 nov. 2011
 *      Author: corum
 */

#include "Triplet.h"

Triplet::Triplet() :
	x(0),
	y(0),
	z(0)
{}

Triplet::Triplet(double x, double y, double z)
{

	this->x = x;
	this->y = y;
	this->z = z;
	if(x == -0)
		this->x = 0;
	if(y == -0)
		this->y = 0;
	if(z == -0)
		this->z = 0;
}

Triplet::Triplet(const Ogre::Vector3& vector)
{
	this->x = Ogre::Math::IFloor(vector.x);
	this->y = Ogre::Math::IFloor(vector.y);
	this->z = Ogre::Math::IFloor(vector.z);
}

Triplet::~Triplet() {}

bool operator==(Triplet const& a, Triplet const& b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

bool operator<(Triplet const& a, Triplet const& b)
{
	if(a.x < b.x)
		return true;
	else if(a.x > b.x)
		return false;
	else if(a.y < b.y)
		return true;
	else if(a.y > b.y)
		return false;
	else if(a.z < b.z)
		return true;
	else
		return false;
}

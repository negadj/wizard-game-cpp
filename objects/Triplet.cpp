/*
 * Triplet.cpp
 *
 *  Created on: 9 nov. 2011
 *      Author: corum
 */

#include "Triplet.h"

Triplet::Triplet() {
	// TODO Auto-generated constructor stub

}

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

Triplet::Triplet(Ogre::Vector3 vector)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
}

Triplet::~Triplet() {
	// TODO Auto-generated destructor stub
}

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

/*
 * Triplet.cpp
 *
 *  Created on: 9 nov. 2011
 *      Author: corum
 */

#include "Triplet.h"

const Triplet Triplet::ZERO = Triplet(0,0,0);

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

Triplet::operator Ogre::Vector3() const{
	return Ogre::Vector3(x,y,z);
}

Ogre::Vector3 round(const Ogre::Vector3 vector)
{
	return Ogre::Vector3(round(vector.x),round(vector.y),round(vector.z));
}

Ogre::Vector3 round(const Ogre::Vector3 vector,int precision)
{
	return Ogre::Vector3(round(vector.x * pow(10, precision))/pow(10, precision),round(vector.y * pow(10, precision))/pow(10, precision),round(vector.z * pow(10, precision))/pow(10, precision));
}

Ogre::Vector3 floor(const Ogre::Vector3 vector)
{
	return Ogre::Vector3(floor(vector.x),floor(vector.y),floor(vector.z));
}

Ogre::Vector3 floor(const Ogre::Vector3 vector,int precision)
{
	return Ogre::Vector3(floor(vector.x * pow(10, precision))/pow(10, precision),floor(vector.y * pow(10, precision))/pow(10, precision),floor(vector.z * pow(10, precision))/pow(10, precision));
}

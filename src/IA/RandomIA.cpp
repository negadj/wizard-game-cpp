/*
 * RandomIA.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "RandomIA.h"

RandomIA::RandomIA(Monster* monster):
	IA(monster)
{}

RandomIA::~RandomIA() {}

Ogre::Vector3 RandomIA::findDirection()
{
	int random = rand()%4;
	if(random == 0)
		return Ogre::Vector3::NEGATIVE_UNIT_Z;
	else if(random == 1)
		return Ogre::Vector3::UNIT_Z;
	else if(random == 2)
		return Ogre::Vector3::NEGATIVE_UNIT_X;
	else
		return Ogre::Vector3::UNIT_X;

}

/*
 * ExplorerIA.cpp
 *
 *  Created on: 25 nov. 2011
 *      Author: corum
 */

#include "ExplorerIA.h"
#include "../ObjectManager.h"

ExplorerIA::ExplorerIA(Monster* monster):
	IA(monster)
{}

ExplorerIA::~ExplorerIA() {}

Ogre::Vector3 ExplorerIA::findDirection()
{
	if(mMonster->getCollisionCorrection().dotProduct(mMonster->getNode()->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z) >= 0)
		return Ogre::Vector3::NEGATIVE_UNIT_Z;
	else
	{
		int i = rand()%3;
		if( i == 0)
			return Ogre::Vector3::UNIT_Z;
		else if(i == 1)
			return Ogre::Vector3::UNIT_X;
		else
			return Ogre::Vector3::NEGATIVE_UNIT_X;

	}

}

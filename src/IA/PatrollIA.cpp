/*
 * PatrollIA.cpp
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#include "PatrollIA.h"
#include "../ObjectManager.h"

PatrollIA::PatrollIA(Monster* monster):
	IA(monster)
{}

PatrollIA::~PatrollIA() {}

Ogre::Vector3 PatrollIA::findDirection()
{
	/*
	 * Detection si la rectification de vitesse est oppose audeplacement. Auquel cas, il y a eu collision.
	 */
	if(mMonster->getCollisionCorrection().dotProduct(mMonster->getFacingDirection()) >= 0)
		return Ogre::Vector3::NEGATIVE_UNIT_Z;
	else
		return Ogre::Vector3::UNIT_Z;

}

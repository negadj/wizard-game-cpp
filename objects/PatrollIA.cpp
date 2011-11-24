/*
 * PatrollIA.cpp
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#include "PatrollIA.h"
#include "ObjectManager.h"

PatrollIA::PatrollIA(Monster* monster):
	IA(monster)
{
	// TODO Auto-generated constructor stub

}

PatrollIA::~PatrollIA() {
	// TODO Auto-generated destructor stub
}

Ogre::Vector3 PatrollIA::findDirection()
{
	if(mMonster->getObjectManager()->isEmpty(mMonster->getNode()->getPosition() + mMonster->getNode()->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z))
		return Ogre::Vector3::NEGATIVE_UNIT_Z;
	else
		return Ogre::Vector3::UNIT_Z;

}

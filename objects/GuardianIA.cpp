/*
 * GuardianIA.cpp
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

/*
 * GardianIA.cpp
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#include "GuardianIA.h"

GuardianIA::GuardianIA(Monster* monster):
	IA(monster)
{
	// TODO Auto-generated constructor stub

}

GuardianIA::~GuardianIA() {
	// TODO Auto-generated destructor stub
}

Ogre::Vector3 GuardianIA::findDirection()
{
	return Ogre::Vector3::ZERO;

}
/*
 * Clock.h
 *
 *  Created on: 14 nov. 2011
 *      Author: corum
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "Ogre.h"

class Clock {

private:
	Ogre::Real mStep;
	Ogre::Real mCurrentTime;

public:
	Clock(double step);
	bool ticked(Ogre::Real &deltaTime);
	Ogre::Real getStep();
};

#endif /* CLOCK_H_ */

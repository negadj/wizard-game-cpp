/*
 * Clock.h
 *
 *  Created on: 14 nov. 2011
 *      Author: corum
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "Ogre.h"
/*
 * Horloge permettant le passage d'un temps dense à un temps discret.
 */
class Clock {

private:
	Ogre::Real mStep;
	Ogre::Real mCurrentTime;

public:
	Clock(double step);
	virtual ~Clock();
	/*
	 * On ajoute à l'horloge le temps deltaTime (en seconde).
	 * Si le temps est suffisant pour provoquer un battement d'horloge,
	 * retourne true et deltaTime contient l'excedant.
	 * Sinon, retourne false et deltaTIme est nul.
	 */
	bool ticked(Ogre::Real &deltaTime);
	Ogre::Real getStep();
};

#endif /* CLOCK_H_ */

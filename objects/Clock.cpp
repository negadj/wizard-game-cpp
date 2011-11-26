/*
 * Clock.cpp
 *
 *  Created on: 14 nov. 2011
 *      Author: corum
 */

#include "Clock.h"

Clock::Clock(double step):
	mStep(step),
	mCurrentTime(0)
{
}

Clock::~Clock() {}

bool Clock::ticked(Ogre::Real &deltaTime)
{
	/* Pour éviter les problèmes dûs à un énorme coup de lag
	 * (comme au chargement de la scène), si deltaTime dépasse 1s,
	 * les calculs physiques sont annulés
	 */
	if (deltaTime > 1)
		return false;

	if(deltaTime > mStep)
	{
		deltaTime -= mStep;
		return true;
	}
	else
	{
		mCurrentTime += deltaTime;
		deltaTime = 0;
		if(mCurrentTime > mStep)
		{
			mCurrentTime -= mStep;
			return true;
		}
		else
			return false;
	}
}

Ogre::Real Clock::getStep()
{
	return mStep;
}

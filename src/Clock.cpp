/*
 * Clock.cpp
 *
 *  Created on: 14 nov. 2011
 *      Author: corum
 */

#include "Clock.h"

#ifdef DEBUG_MODE
#define LOG(x) Ogre::LogManager::getSingleton().logMessage(x)
#endif

Clock::Clock(double step):
	mStep(step),
	mCurrentTime(0)
{
}

Clock::~Clock() {
#ifdef DEBUG_MODE
LOG("call Clock destructor");
#endif
}

bool Clock::ticked(Ogre::Real &deltaTime)
{
	/* Pour éviter les problèmes dûs à un énorme coup de freeze,
	 * si deltaTime dépasse 1s, les calculs physiques sont annulés
	 */
	if (deltaTime > 50 * mStep) {
#ifdef DEBUG_MODE
LOG("TICK SKIPPED !");
#endif
		return false;
	}

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

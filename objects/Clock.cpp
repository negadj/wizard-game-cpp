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

bool Clock::ticked(Ogre::Real &deltaTime)
{
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

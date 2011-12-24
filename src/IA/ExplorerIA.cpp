/*
 * ExplorerIA.cpp
 *
 *  Created on: 25 nov. 2011
 *      Author: corum
 */

#include "ExplorerIA.h"
#include "../ObjectManager.h"

ExplorerIA::ExplorerIA(AnimatedObject* character):
	IA(character)
{}

ExplorerIA::~ExplorerIA() {}

void ExplorerIA::takeADecision() {
	if(mCharacter->getCollisionCorrection().dotProduct(mCharacter->getFacingDirection()) >= 0)
		mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionForward);
	else
	{
		int i = rand()%3;
		if( i == 0)
			mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionBackward);
		else if(i == 1)
			mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionLeft);
		else
			mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionRight);

	}
	mCharacter->setPropulsionLocalDirection(PhysicalObject::LocalDirectionForward);
}

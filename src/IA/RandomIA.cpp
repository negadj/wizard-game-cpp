/*
 * RandomIA.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "RandomIA.h"

RandomIA::RandomIA(AnimatedObject* character):
	IA(character)
{}

RandomIA::~RandomIA() {}

void RandomIA::takeADecision() {
	int random = rand()%4;
	if(random == 0)
		mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionForward);
	else if(random == 1)
		mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionBackward);
	else if(random == 2)
		mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionLeft);
	else
		mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionRight);

	mCharacter->setPropulsionLocalDirection(PhysicalObject::LocalDirectionForward);
}

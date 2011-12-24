/*
 * PatrollIA.cpp
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#include "PatrollIA.h"
#include "../ObjectManager.h"

PatrollIA::PatrollIA(AnimatedObject* character):
	IA(character)
{}

PatrollIA::~PatrollIA() {}

void PatrollIA::takeADecision() {
	/*
	 * Detection si la rectification de vitesse est oppose au deplacement. Auquel cas, il y a eu collision.
	 */
	if(mCharacter->getCollisionCorrection().dotProduct(mCharacter->getFacingDirection()) >= 0)
		mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionForward);
	else
		mCharacter->faceLocalDirection(PhysicalObject::LocalDirectionBackward);

	mCharacter->setPropulsionLocalDirection(PhysicalObject::LocalDirectionForward);
}

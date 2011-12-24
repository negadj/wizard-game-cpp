/*
 * IA.cpp
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#include "IA.h"
#include "RandomIA.h"
#include "GuardianIA.h"
#include "PatrollIA.h"
#include "ExplorerIA.h"
IA::IA(AnimatedObject* character):
	mCharacter(character)
{}

IA::~IA() {}

AnimatedObject* IA::getCharacter() {
	return mCharacter;
}

IA* IA::getRandomIA(AnimatedObject* character)
{
	int i = rand() % 4;
	if(i == 0)
	{
		return new GuardianIA(character);
	}
	else if(i == 1)
	{
		return new RandomIA(character);
	}
	else if(i == 2)
	{
		return new PatrollIA(character);
	}
	else
	{
		return new ExplorerIA(character);
	}
}

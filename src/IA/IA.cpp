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
IA::IA(Monster* monster):
	mMonster(monster)
{}

IA::~IA() {}

IA* IA::getIA(Monster* monster)
{
	int i = 3; //rand()%3;
	if(i == 0)
	{
		return new GuardianIA(monster);
	}
	else if(i == 1)
	{
		return new RandomIA(monster);
	}
	else if(i == 2)
	{
		return new PatrollIA(monster);
	}
	else
	{
		return new ExplorerIA(monster);
	}
}

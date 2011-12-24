/*
 * Monster.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "Monster.h"
#include "../ObjectManager.h"
#include "../IA/ExplorerIA.h"

Monster::Monster(Ogre::SceneNode* originNode):
	AnimatedObject(originNode, "robot.mesh", Ogre::Vector3(0.45,0.9,0.45), -0.9, 0.02, Ogre::Degree(90), "Monster")
{
	registerType(TYPE_HOSTILE);
	registerAnimation("Walk");
	setIA(new ExplorerIA(this));
}

Monster::~Monster() {}

void Monster::doPreCollisionUpdate(Ogre::Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter Monster::doPreCollisionUpdate");
#endif
//	/* Change aléatoirement de stratégie */
//	if(rand()%100 == 0)
//		setIA(IA::getRandomIA(this));

	AnimatedObject::doPreCollisionUpdate(deltaTime);

#ifdef DEBUG_MODE
LOG("exit Monster::doPreCollisionUpdate");
#endif
}

void Monster::doPostCollisionUpdate(Ogre::Real deltaTime)
{
	if (getSpeed().isZeroLength())
		enableAnimation("Walk");
	else
		disableAnimation("Walk");
	AnimatedObject::doPostCollisionUpdate(deltaTime);
}

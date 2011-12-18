/*
 * Monster.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "Monster.h"
#include "../ObjectManager.h"


Monster::Monster(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name):
	AnimatedObject(objectManager, originNode, name, TYPE_HOSTILE, "robot.mesh", Ogre::Vector3(0.45,0.9,0.45), -0.9, 0.02, Ogre::Degree(90), "Monster"),
	mIA(IA::getIA(this))
{
	registerAnimation("Walk");
}

Monster::~Monster() {
	//getNode()->removeAndDestroyChild(mBodyNode->getName());
	delete mIA;
}

void Monster::doPreCollisionUpdate(Ogre::Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter Monster::doPreCollisionUpdate");
#endif
	/* Change aléatoirement de stratégie */
	if(rand()%100 == 0)
	{
		delete mIA;
		mIA = IA::getIA(this);
	}

	Ogre::Vector3 relativeDirection = mIA->findDirection();

	if(isOnGround() && relativeDirection != Ogre::Vector3::ZERO)
	{
		/* Reorientation du monstre */
		getNode()->setDirection(relativeDirection,Ogre::Node::TS_LOCAL);

		/* Ajout de la force motrice*/
		addForce(getPropulsion() * (getNode()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z));
	}

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

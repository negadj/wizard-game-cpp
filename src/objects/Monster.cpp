/*
 * Monster.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "Monster.h"
#include "../ObjectManager.h"


Monster::Monster(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name):
	AnimatedObject(objectManager, originNode, name, TYPE_HOSTILE, "robot.mesh", Ogre::Vector3(0.45,0.9,0.45),"Monster"),
	mBodyNode(0),
	mIA(IA::getIA(this))
{
	registerAnimation("Walk");
	setupBody(originNode);
}

Monster::~Monster() {
	getNode()->removeAndDestroyChild(mBodyNode->getName());
	delete mIA;
}

void Monster::setupBody(Ogre::SceneNode* originNode) {
	// Entités pour le corps

	// Création du corps du personnage
	mBodyNode = getNode()->createChildSceneNode(getName()+"_body", Ogre::Vector3::NEGATIVE_UNIT_Y * 0.9);
	mBodyNode->scale(Ogre::Vector3::UNIT_SCALE * 0.02);
	mBodyNode->yaw(Ogre::Degree(90));
	mBodyNode->setInitialState();

	getEntity()->getAnimationState("Walk")->setEnabled(true);
	/* Le constructeur de PhysicalObject attache mEntity à mNode,
	   ce n'est pas ce que l'on souhaite ici */
#ifdef _WINDOWS
    getEntity()->getParentSceneNode()->detachObject(getEntity());
#else
	getEntity()->detachFromParent();
#endif
	mBodyNode->attachObject(getEntity());
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
	AnimatedObject::doPostCollisionUpdate(deltaTime);
}

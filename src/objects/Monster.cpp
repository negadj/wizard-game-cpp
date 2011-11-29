/*
 * Monster.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "Monster.h"
#include "../ObjectManager.h"


Monster::Monster(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name):
	AnimatedObject(objectManager, originNode, name, 2, "robot.mesh", Ogre::Vector3(0.45,0.9,0.45),"Monster"),
	mBodyNode(0),
	mIA(IA::getIA(this))
{
	registerAnimation("Walk");
	setupBody(originNode);
}

Monster::~Monster() {
	delete mIA;
}

void Monster::setupBody(Ogre::SceneNode* originNode) {
	// Entités pour le corps

	// Création du corps du personnage
	mBodyNode = getNode()->createChildSceneNode(Ogre::Vector3::NEGATIVE_UNIT_Y * 0.9);
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

void Monster::preCollisionUpdate(Ogre::Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter Monster::preCollisionUpdate");
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

	AnimatedObject::preCollisionUpdate(deltaTime);

#ifdef DEBUG_MODE
LOG("exit Monster::preCollisionUpdate");
#endif
}

void Monster::postCollisionUpdate(Ogre::Real deltaTime)
{
	AnimatedObject::postCollisionUpdate(deltaTime);
}

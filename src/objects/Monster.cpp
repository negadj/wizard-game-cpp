/*
 * Monster.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "Monster.h"
#include "../ObjectManager.h"


Monster::Monster(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name):
	PhysicalObject(objectManager, originNode, name, 2, "Sinbad.mesh", Ogre::Vector3(0.45,0.9,0.45),"Monster"),
	mBodyNode(0),
	mSword1(0),
	mSword2(0),
	mDirection(Ogre::Vector3::ZERO),
	mPropulsion(40),
	mVerticalVelocity(0),
	mIA(IA::getIA(this))
{
	setupBody(originNode);
}

Monster::~Monster() {
	delete mIA;
}

void Monster::setupBody(Ogre::SceneNode* originNode) {
	// Entités pour le corps

	// Création du corps du personnage
	mBodyNode = getNode()->createChildSceneNode(Ogre::Vector3::UNIT_Y * 0.05);
	mBodyNode->scale(Ogre::Vector3::UNIT_SCALE * 0.2);
	mBodyNode->yaw(Ogre::Degree(180));
	mBodyNode->setInitialState();

	/* Le constructeur de PhysicalObject attache mEntity à mNode,
	   ce n'est pas ce que l'on souhaite ici */
#ifdef _WINDOWS
    getEntity()->getParentSceneNode()->detachObject(getEntity());
#else
	getEntity()->detachFromParent();
#endif
	mBodyNode->attachObject(getEntity());
}

void Monster::update(Ogre::Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter Monster::update");
#endif
	/* Mise à jour de la vitesse du joueur en fonction des touches,
	 dans le référentiel global. */
	if(rand()%100 == 0)
	{
#ifdef DEBUG_MODE
LOG("monster : changement d'IA");
#endif
		delete mIA;
		mIA = IA::getIA(this);
#ifdef DEBUG_MODE
LOG("monster : IA changée");
#endif
	}
	Ogre::Vector3 relativeDirection = mIA->findDirection();
#ifdef DEBUG_MODE
LOG("monster : direction choisie");
#endif
	if(isOnGround() && relativeDirection != Ogre::Vector3::ZERO)
	{
		/* Reorientation du monstre */
		getNode()->setDirection(relativeDirection,Ogre::Node::TS_LOCAL);
		addSpeed(deltaTime * (-getSpeed()*getObjectManager()->getStrench(this) + getObjectManager()->getGravity(this)+  mPropulsion * (getNode()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z)));
	}
	else
		addSpeed(deltaTime * (-getSpeed()*getObjectManager()->getStrench(this) + getObjectManager()->getGravity(this)));
#ifdef DEBUG_MODE
LOG("exit Monster::update");
#endif
}

/*
 * Monster.cpp
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#include "Monster.h"
#include "ObjectManager.h"

Monster::Monster(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name):
	PhysicalObject(objectManager, originNode, name, 2, "Sinbad.mesh", Ogre::Vector3(0.45,0.9,0.45),"Monster"),
	mBodyNode(0),
	mSword1(0),
	mSword2(0),
	mDirection(Ogre::Vector3::ZERO),
	mPropulsion(40),
	mVerticalVelocity(0)
{
	setupBody(originNode);
}

Monster::~Monster() {
	// TODO Auto-generated destructor stub
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
	getEntity()->detachFromParent();
	mBodyNode->attachObject(getEntity());
}

void Monster::update(Ogre::Real deltaTime) {
	/* Mise à jour de la vitesse du joueur en fonction des touches,
	 dans le référentiel global. */
	if(getObjectManager()->isOnGround(this))
		addSpeed(deltaTime * (-getSpeed()*getObjectManager()->getStrench(this) + getObjectManager()->getGravity(this)+  mPropulsion * (getNode()->getOrientation() * mDirection.normalisedCopy())));
	else
		addSpeed(deltaTime * (-getSpeed()*getObjectManager()->getStrench(this) + getObjectManager()->getGravity(this)));
}

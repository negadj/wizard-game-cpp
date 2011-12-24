/*
 * Wizard.cpp
 *
 *  Created on: 23 déc. 2011
 *      Author: gecko
 */

#include "Wizard.h"

Wizard::Wizard(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name) :
	AnimatedObject(objectManager, originNode, name, "Sinbad.mesh", Ogre::Vector3(0.45,0.9,0.45), -0.05, 0.2, Ogre::Degree(180), "Joueur"),
	mSword1(0),
	mSword2(0)
{
	registerType(TYPE_FRIENDLY);
	getNode()->translate(1,1.5,1);
	getNode()->setInitialState();
	mSword1 = originNode->getCreator()->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = originNode->getCreator()->createEntity("SinbadSword2", "Sword.mesh");
	getEntity()->attachObjectToBone("Sheath.L", mSword1);
	getEntity()->attachObjectToBone("Sheath.R", mSword2);
	registerAnimation("RunBase");
	registerAnimation("RunTop");
}

Wizard::~Wizard() {
	//getNode()->removeAndDestroyChild(mBodyNode->getName());
	getNode()->getCreator()->destroyEntity(mSword1);
	getNode()->getCreator()->destroyEntity(mSword2);
}

void Wizard::doPreCollisionUpdate(Ogre::Real deltaTime) {
	/* Mise à jour de la vitesse du joueur en fonction des touches,
	 dans le référentiel global. */
//	if(isOnGround())
//		/* Ajout de la force motrice */
//		addForce((getPropulsion() * (getNode()->getOrientation() * mDirection.normalisedCopy())));
//    else
//        addForce((getPropulsion() * (getNode()->getOrientation() * mDirection.normalisedCopy()))/15);


	AnimatedObject::doPreCollisionUpdate(deltaTime);
}

void Wizard::doPostCollisionUpdate(Ogre::Real deltaTime)
{
	if (getSpeed().isZeroLength()) {
		disableAnimation("RunTop");
		disableAnimation("RunBase");
	}
	else {
		enableAnimation("RunTop");
		enableAnimation("RunBase");
	}
	AnimatedObject::doPostCollisionUpdate(deltaTime);
}

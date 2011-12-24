/*
 * AnimatedObject.cpp
 *
 *  Created on: 29 nov. 2011
 *      Author: corum
 */

#include "AnimatedObject.h"
#include "../Triplet.h"
#include "../IA/IA.h"
#include "../IA/GuardianIA.h"

AnimatedObject::AnimatedObject(ObjectManager* objectManager, Ogre::SceneNode* node, Ogre::String name, Ogre::String meshName, Ogre::Vector3 volume, const Ogre::Real& meshHeight, const Ogre::Real& meshScale, const Ogre::Radian& meshOrientation, std::string description, PhysicalMaterial material):
	PhysicalObject(objectManager, node, name, meshName, volume, material, description),
	mBodyNode(NULL),
	mVelocity(40),
	mAnimations(std::vector<Ogre::String>()),
	mIA(new GuardianIA(this)),
	mPropulsionDir(Ogre::Vector3::ZERO)
{
	registerType(TYPE_ANIMATED);
	setActive(true);
	setupBody(node, volume, meshHeight, meshScale, meshOrientation);
}

void AnimatedObject::setupBody(Ogre::SceneNode* node, Ogre::Vector3 volume, const Ogre::Real& meshHeight, const Ogre::Real& meshScale, const Ogre::Radian& meshOrientation) {
	// Création du corps de l'objet
	mBodyNode = getNode()->createChildSceneNode(getName()+"_body", Ogre::Vector3::UNIT_Y * meshHeight);
	mBodyNode->scale(Ogre::Vector3::UNIT_SCALE * meshScale);
	mBodyNode->yaw(meshOrientation);
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

AnimatedObject::~AnimatedObject()
{
	delete mIA;
}

IA* AnimatedObject::getIA() const {
	return mIA;
}
void AnimatedObject::setIA(IA* ia, bool removeOldIA) {
	if (removeOldIA)
		delete mIA;
	mIA = ia;
}


void AnimatedObject::enableAnimation(Ogre::String animationName) {
	getEntity()->getAnimationState(animationName)->setEnabled(true);
}

void AnimatedObject::disableAnimation(Ogre::String animationName) {
	getEntity()->getAnimationState(animationName)->setEnabled(false);
}

void AnimatedObject::doPreCollisionUpdate(Ogre::Real deltaTime)
{
	mIA->takeADecision();

	if(mPropulsionDir != Ogre::Vector3::ZERO)
	{
		/* Ajout de la force motrice */
		if(isOnGround())
			addForce(getPropulsion());
		else
			addForce(getPropulsion()/15);
	}

	PhysicalObject::doPreCollisionUpdate(deltaTime);
}

void AnimatedObject::doPostCollisionUpdate(Ogre::Real deltaTime)
{
	PhysicalObject::doPostCollisionUpdate(deltaTime);
	for(std::vector<Ogre::String>::iterator it = mAnimations.begin() ; it != mAnimations.end() ; ++it)
	{
		if(round(getSpeed(),1) != Ogre::Vector3::ZERO)
		{
			if (getSpeed().z > 0) { //TODO: formule probablement fausse, getSpeed donne un vecteur dans le référentiel global
				getEntity()->getAnimationState(*it)->addTime(-deltaTime);
			}
			else {
				getEntity()->getAnimationState(*it)->addTime(deltaTime);
			}
		}
		else
		{
			getEntity()->getAnimationState(*it)->setTimePosition(0); //RAZ de l'animation
		}

	}
}

Ogre::Real AnimatedObject::getVelocity() {
	return mVelocity;
}

void AnimatedObject::setVelocity(const Ogre::Real& velocity) {
	mVelocity = velocity;
}

void AnimatedObject::setPropulsionDirection(const Ogre::Vector3& direction) {
	mPropulsionDir = direction.normalisedCopy(); // Note : un vecteur nul reste un vecteur nul
}

void AnimatedObject::setPropulsionLocalDirection(const Ogre::Vector3& direction) {
	mPropulsionDir = (getOrientation() * direction).normalisedCopy(); // Note : un vecteur nul reste un vecteur nul
}

Ogre::Vector3 AnimatedObject::getPropulsion() {
	return mPropulsionDir * mVelocity;
}


void AnimatedObject::registerAnimation(Ogre::String animation)
{
	mAnimations.push_back(animation);
}

/*
 * AnimatedObject.cpp
 *
 *  Created on: 29 nov. 2011
 *      Author: corum
 */

#include "AnimatedObject.h"
#include "../Triplet.h"

AnimatedObject::AnimatedObject(ObjectManager* objectManager, Ogre::SceneNode* node, Ogre::String name, int id, Ogre::String meshName, Ogre::Vector3 volume, std::string description):
	PhysicalObject(objectManager,node,name,id,meshName,volume,description),
	mPropulsion(40)
{

}

AnimatedObject::~AnimatedObject()
{

}

void AnimatedObject::preCollisionUpdate(Ogre::Real deltaTime)
{
	PhysicalObject::preCollisionUpdate(deltaTime);
}

void AnimatedObject::postCollisionUpdate(Ogre::Real deltaTime)
{
	PhysicalObject::postCollisionUpdate(deltaTime);
	for(std::vector<Ogre::String>::iterator it = mAnimations.begin() ; it != mAnimations.end() ; ++it)
	{
		std::cout << getSpeed() << std::endl;
		if(round(getSpeed(),1) != Ogre::Vector3::ZERO)
		{
			getEntity()->getAnimationState(*it)->setEnabled(true);
			if (getSpeed().z > 0) {
				getEntity()->getAnimationState(*it)->addTime(-deltaTime);
			}
			else {
				getEntity()->getAnimationState(*it)->addTime(deltaTime);
			}
		}
		else
		{
			getEntity()->getAnimationState(*it)->setEnabled(false);
			getEntity()->getAnimationState(*it)->setTimePosition(0); //RAZ de l'animation
		}

	}
}

Ogre::Real AnimatedObject::getPropulsion()
{
	return mPropulsion;
}

void AnimatedObject::registerAnimation(Ogre::String animation)
{
	mAnimations.push_back(animation);
}

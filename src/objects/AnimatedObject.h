/*
 * AnimatedObject.h
 *
 *  Created on: 29 nov. 2011
 *      Author: corum
 */

#ifndef ANIMATEDOBJECT_H_
#define ANIMATEDOBJECT_H_

#include <Ogre.h>
#include "PhysicalObject.h"

class AnimatedObject: public PhysicalObject {
public:
	AnimatedObject(ObjectManager* objectManager, Ogre::SceneNode* node, Ogre::String name, int id, Ogre::String meshName, Ogre::Vector3 volume, std::string description = "Objet animé");
	virtual ~AnimatedObject();
	Ogre::Real getPropulsion();

protected:
	void registerAnimation(Ogre::String AnimationName);
	virtual void doPreCollisionUpdate(Ogre::Real deltaTime);
	virtual void doPostCollisionUpdate(Ogre::Real deltaTime);

private:
	Ogre::Real mPropulsion;
	std::vector<Ogre::String> mAnimations;
};

#endif /* ANIMATEDOBJECT_H_ */

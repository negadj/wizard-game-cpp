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
	AnimatedObject(ObjectManager* objectManager, Ogre::SceneNode* node, Ogre::String name, ObjectType type, Ogre::String meshName, Ogre::Vector3 volume, const Ogre::Real& meshHeight, const Ogre::Real& meshScale, const Ogre::Radian& meshOrientation, std::string description = "Objet animé", PhysicalMaterial material = PhysicalMaterial::Flesh);
	virtual ~AnimatedObject();
	Ogre::Real getPropulsion();

protected:
	void registerAnimation(Ogre::String AnimationName);
	void enableAnimation(Ogre::String animationName);
	void disableAnimation(Ogre::String animationName);
	virtual void doPreCollisionUpdate(Ogre::Real deltaTime);
	virtual void doPostCollisionUpdate(Ogre::Real deltaTime);

private:
	Ogre::SceneNode* mBodyNode;
	Ogre::Real mPropulsion;
	std::vector<Ogre::String> mAnimations;

	void setupBody(Ogre::SceneNode* node, Ogre::Vector3 volume, const Ogre::Real& meshHeight, const Ogre::Real& meshScale, const Ogre::Radian& meshOrientation);
};

#endif /* ANIMATEDOBJECT_H_ */

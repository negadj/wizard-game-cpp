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

class IA;

class AnimatedObject: public PhysicalObject {
	friend class Player; // TODO: trouver moins permissif
public:
	AnimatedObject(Ogre::SceneNode* node, Ogre::String meshName, Ogre::Vector3 volume, const Ogre::Real& meshHeight, const Ogre::Real& meshScale, const Ogre::Radian& meshOrientation, std::string description = "Objet animé", PhysicalMaterial material = PhysicalMaterial::Flesh);
	virtual ~AnimatedObject();
	Ogre::Real getVelocity();
	void setVelocity(const Ogre::Real& velocity);
	/*
	 * Indique la direction dans laquelle on souhaite se déplacer (référentiel absolu).
	 * Attention : le vecteur est stocké normalisé
	 */
	void setPropulsionDirection(const Ogre::Vector3& direction);
	void setPropulsionLocalDirection(const Ogre::Vector3& direction);
	Ogre::Vector3 getPropulsion();
	IA* getIA() const;
	void setIA(IA* ia, bool removeOldIA = true);

protected:
	void registerAnimation(Ogre::String AnimationName);
	void enableAnimation(Ogre::String animationName);
	void disableAnimation(Ogre::String animationName);
	virtual void doPreCollisionUpdate(Ogre::Real deltaTime);
	virtual void doPostCollisionUpdate(Ogre::Real deltaTime);

private:
	Ogre::SceneNode* mBodyNode;
	Ogre::Real mVelocity;
	std::vector<Ogre::String> mAnimations;
	IA* mIA;
	Ogre::Vector3 mPropulsionDir;

	void setupBody(Ogre::SceneNode* node, Ogre::Vector3 volume, const Ogre::Real& meshHeight, const Ogre::Real& meshScale, const Ogre::Radian& meshOrientation);
};

#endif /* ANIMATEDOBJECT_H_ */

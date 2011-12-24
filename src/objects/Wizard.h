/*
 * Wizard.h
 *
 *  Created on: 23 déc. 2011
 *      Author: gecko
 */

#ifndef WIZARD_H_
#define WIZARD_H_

#include "AnimatedObject.h"

class Wizard : public AnimatedObject {
	friend class ObjectManager;

protected:
	Wizard(Ogre::SceneNode* originNode);
	virtual void doPostCollisionUpdate(Ogre::Real deltaTime);
	virtual void doPreCollisionUpdate(Ogre::Real deltaTime);
	virtual ~Wizard();

private:
	Ogre::Entity* mSword1;
	Ogre::Entity* mSword2;
};

#endif /* WIZARD_H_ */

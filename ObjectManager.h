/*
 * PhysicManager.h
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "Ogre.h"
#include "CollisionTools.h"

class ObjectManager {
public:
	ObjectManager(Ogre::SceneManager* scnMgr);
	~ObjectManager();
private:
	Ogre::SceneManager* mSceneMgr;
//	Ogre::RaySceneQuery* mRSQ;
	MOC::CollisionTools mCollisionTools;
};

#endif /* OBJECTMANAGER_H_ */

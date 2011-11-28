/*
 * CollisionManager.h
 *
 *  Created on: 28 nov. 2011
 *      Author: gecko
 */

#ifndef COLLISIONMANAGER_H_
#define COLLISIONMANAGER_H_

#include "CollisionTools.h"
#include <Ogre.h>
#include "objects/PhysicalObject.h"

//forward declaration
class ObjectManager;

class CollisionManager {
private:
	ObjectManager* mObjMgr;
	MOC::CollisionTools mCollisionTools;
public:
	CollisionManager(ObjectManager* objMgr, Ogre::SceneManager* scnMgr);
	virtual ~CollisionManager();

	void moveWithCollisions(PhysicalObject* &obj, const Ogre::Real deltaTime);
	Ogre::Vector3 handleCollision(const PhysicalObject* obj, Ogre::Vector3 deplacement);
	bool raycast(const Ogre::Vector3 &point, const Ogre::Vector3 &normal, Ogre::Vector3 &result, Ogre::Entity* &target,
				float &closest_distance, Ogre::Vector3 &polygon_normal, const Ogre::Entity* source = 0, const Ogre::uint32 queryMask = 0xFFFFFFFF);
};

#endif /* COLLISIONMANAGER_H_ */

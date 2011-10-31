/*
 * PhysicManager.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "ObjectManager.h"

ObjectManager::ObjectManager(Ogre::SceneManager* scnMgr) :
	mSceneMgr(scnMgr),
//	mRSQ(scnMgr->creater),
	mCollisionTools(scnMgr)
{}

ObjectManager::~ObjectManager() {
	// On détruit tout les objets référencés
	for(std::vector<PhysicalObject*>::iterator it = mObjects.begin();
			it != mObjects.end(); ++it) {
		delete *it;
	}
}

void ObjectManager::updateObjects(Ogre::Real deltaTime) {
	for(std::vector<PhysicalObject*>::iterator it = mActiveObjects.begin();
			it != mActiveObjects.end(); ++it) {
		// On lance d'abord les update personalisés de chaque objet.
		(*it)->update(deltaTime);

		// Puis on calcule les nouvelles positions des objets.
		(*it)->getNode()->translate((*it)->getSpeed()*deltaTime);
	}
}

Player* ObjectManager::createPlayer(Ogre::Camera* camera) {
	Player* p = new Player(camera);
	mObjects.push_back(p);
	mActiveObjects.push_back(p);
	return p;
}

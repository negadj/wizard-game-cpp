/*
 * PhysicManager.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "ObjectManager.h"

unsigned long ObjectManager::_countObject = 0;

ObjectManager::ObjectManager(Ogre::SceneManager* scnMgr) :
	mSceneMgr(scnMgr),
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
	Player* p = new Player(Ogre::StringConverter::toString(++_countObject), camera);
	mObjects.push_back(p);
	mActiveObjects.push_back(p);
	return p;
}

Block* ObjectManager::createBlock(Ogre::Vector3 position) {
	Ogre::String name = Ogre::StringConverter::toString(++_countObject);
	Block* b = new Block(mSceneMgr->getRootSceneNode()->createChildSceneNode(name, position), name, 2);
	mObjects.push_back(b);
	return b;
}

void ObjectManager::loadScene() {
	for (int i=0; i<100; i++) {
		for (int j=0; j<100; j++) {
			for (int k=0; k<2; k++) {
				if (rand() < (RAND_MAX/3)) {
					createBlock(Vector3(i,k,-j) * 2);
				}
			}
		}
	}
}

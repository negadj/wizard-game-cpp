/*
 * ObjectManager.cpp
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
	for(std::map<std::string,PhysicalObject*>::iterator it = mObjects.begin();
			it != mObjects.end(); ++it) {
		delete (it->second);
	}
}

void ObjectManager::moveWithCollisions(PhysicalObject* &obj, const Ogre::Real deltaTime)
{
    // Calcul des nouvelles positions des objets.
    // On gère les éventuelles collisions
    if(obj->getSpeed() != Ogre::Vector3::ZERO){
    	int counter =0;
        float dist = 0.0f;
        Ogre::Vector3 destination = Ogre::Vector3::ZERO;
        Ogre::MovableObject *target;
        Ogre::Vector3 polygon_normal;
        Ogre::Vector3 distance = obj->getSpeed() * deltaTime;
        while(mCollisionTools.raycastFromPoint(obj->getNode()->getPosition(), distance.normalisedCopy(), destination, target, dist, polygon_normal)
        	&& dist < distance.length())
        {
        	// TODO: Des fois, la boucle se lock. Un compteur permet d'éviter un freeze complet
        	if (counter > 10)
        		break;
        	//std::cout << target->getName() << std::endl;
        	if(distance.dotProduct(polygon_normal) < 0)
        		polygon_normal = -polygon_normal;
        	distance -= polygon_normal.dotProduct(distance - distance.normalisedCopy() * dist) * polygon_normal; //destination);
        	counter+=1;
        }


        obj->getNode()->translate(distance); //TODO: TS_WORLD ?
        obj->setSpeed(distance/deltaTime);
    }
}

bool ObjectManager::objectReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, PhysicalObject* &target) {
	target = NULL;
	Ogre::Real dist = 0;
	Ogre::Vector3 pos;
	Ogre::Entity* entity;
	Ogre::Vector3 polygon_normal;
	if (mCollisionTools.raycastFromPoint(from, normal, pos, entity, dist,polygon_normal)
		&& dist < reachRadius) {
		std::map<std::string, PhysicalObject*>::iterator it = mObjects.find(entity->getParentSceneNode()->getName());
		if (it != mObjects.end()) {
			target = it->second;
			std::cout << target->getName() << " " << entity->getParentSceneNode()->getName() << std::endl;
			return true;
		}
	}
	return false;
}

void ObjectManager::updateObjects(Ogre::Real deltaTime) {
	PhysicalObject* obj = NULL;
	for(std::vector<PhysicalObject*>::iterator it = mActiveObjects.begin();
			it != mActiveObjects.end(); ++it) {
		obj = *it;

		// On lance d'abord les update personalisés de chaque objet
		// (pour les animations, modifications de vitesse, etc...).
		obj->update(deltaTime);

		// Calcul des nouvelles positions des objets.
		moveWithCollisions(obj, deltaTime);
	}
}

Player* ObjectManager::createPlayer(Ogre::Camera* camera) {
	Ogre::String name = Ogre::StringConverter::toString(++_countObject);
	Player* p = new Player(this, name, camera);
	mObjects[name] = p;
	mActiveObjects.push_back(p);
	return p;
}

Block* ObjectManager::createBlock(const Ogre::Vector3 position) {
	Ogre::String name = Ogre::StringConverter::toString(++_countObject);
	Block* b = new Block(this, mSceneMgr->getRootSceneNode(), name, 2);
	b->getNode()->setPosition(position);
	//TODO: setInitialState ?
	mObjects[name] = b;
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

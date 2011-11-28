/*
 * ObjectManager.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "ObjectManager.h"
#include <algorithm>

unsigned long ObjectManager::_countObject = 0;

ObjectManager::ObjectManager(Ogre::SceneManager* scnMgr) :
	mSceneMgr(scnMgr),
	mCollisionMgr(this, scnMgr),
	mObjects(std::map<std::string,PhysicalObject*>()),
	mActiveObjects(std::vector<PhysicalObject*>()),
	mTerrain(this, scnMgr->createStaticGeometry("terrain")),
	mPhysicalClock(Clock(0.02)),
	mMapManager(10)
{}

ObjectManager::~ObjectManager()
{
#ifdef DEBUG_MODE
LOG("enter ObjectManager destructor");
#endif
	// On détruit tout les objets référencés
		mSceneMgr->clearScene();
#ifdef DEBUG_MODE
LOG("scene cleared");
#endif
	for(std::map<std::string,PhysicalObject*>::iterator it = mObjects.begin();
			it != mObjects.end(); ++it) {
		delete (it->second);
	}
#ifdef DEBUG_MODE
LOG("objects deleted");
#endif
	mActiveObjects.clear();
	mObjects.clear();
#ifdef DEBUG_MODE
LOG("exit ObjectManager destructor");
#endif
}

PhysicalObject* ObjectManager::getObject(const Ogre::String& name) {
#ifdef DEBUG_MODE
LOG("call ObjectManager::getObject");
#endif
	if (mObjects.find(name) == mObjects.end())
		return NULL;
	return mObjects[name];
}

bool ObjectManager::objectReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, PhysicalObject* &target) {
#ifdef DEBUG_MODE
LOG("call ObjectManager::objectReached");
#endif
	target = NULL;
	Ogre::Real dist = 0;
	Ogre::Vector3 pos;
	Ogre::Entity* entity;
	Ogre::Vector3 polygon_normal;

	// On cherche si un objet actif est atteint
	if (mCollisionMgr.raycast(from, normal, pos, entity, dist, polygon_normal)
		&& dist < reachRadius) {
		std::map<std::string, PhysicalObject*>::iterator it = mObjects.find(entity->getParentSceneNode()->getName());
		if (it != mObjects.end()) {
			target = it->second;
			return true;
		}
	}
	return false;
}

bool ObjectManager::blockReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, Block* &target) {
#ifdef DEBUG_MODE
LOG("call ObjectManager::blockReached");
#endif
	target = NULL;
	// On trouve le premier bloc libre sur la trajectoire
	// en regardant régulièrement le long de la direction visée.
	// TODO: trouver un meilleur algo
	Ogre::Vector3 currentPos;
	int i = from.x, j = from.y, k = from.z;
	for (int step = 0; step < int(reachRadius*10); step++) {
		currentPos = from + (double(step) / 10) * normal;
		if (i != int(currentPos.x + 0.5) || j != int(currentPos.y + 0.5) || k != int(currentPos.z + 0.5)) {
			i = int(currentPos.x + 0.5);
			j = int(currentPos.y + 0.5);
			k = int(currentPos.z + 0.5);
			if (!mTerrain.isFree(Triplet(i,j,k))) {
				target = mTerrain.getBlock(Triplet(i,j,k));
				return true;
			}
		}
	}
	return false;
}

void ObjectManager::updateObjects(Ogre::Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter ObjectManager::updateObjects");
LOG("nbr d'objets : " + Ogre::StringConverter::toString(mObjects.size()) + ", actifs : " + Ogre::StringConverter::toString(mActiveObjects.size()));
#endif
	PhysicalObject* obj = NULL;
	while(mPhysicalClock.ticked(deltaTime)){
#ifdef DEBUG_MODE
LOG("tick");
#endif
		for(std::vector<PhysicalObject*>::iterator it = mActiveObjects.begin();
			it != mActiveObjects.end(); ++it) {
			obj = *it;

			// On lance d'abord les update personalisés de chaque objet
		// (pour les animations, modifications de vitesse, etc...).

			obj->update(mPhysicalClock.getStep());
		// Calcul des nouvelles positions des objets.
			mCollisionMgr.moveWithCollisions(obj, mPhysicalClock.getStep());
		}
	}
	for(std::vector<PhysicalObject*>::iterator it = mActiveObjects.begin(); it != mActiveObjects.end();++it)
	{
		obj = *it;
		if(obj->getId() == (ObjectId_t) 1)
		{
			PhysicalObject* obj2 = NULL;
			for(std::vector<PhysicalObject*>::iterator it2 = mActiveObjects.begin(); it2 != mActiveObjects.end();++it2)
			{
				obj2 = *it2;
				if(obj != obj2)
				{
					if((obj2->getNode()->getPosition() - obj->getNode()->getPosition()).length() < 0.9)
					{
					#ifdef DEBUG_MODE
						LOG("CONTACT !");
					#endif
						obj->setIntegrity(0);
					}
				}
			}
		}
	}
#ifdef DEBUG_MODE
LOG("exit ObjectManager::updateObjects");
#endif
}

Player* ObjectManager::createPlayer(Ogre::Camera* camera) {
#ifdef DEBUG_MODE
LOG("enter ObjectManager::createPlayer");
#endif
	Ogre::String name = Ogre::StringConverter::toString(++_countObject);
	Player* p = new Player(this, name, camera);
	mObjects[name] = p;
	mActiveObjects.push_back(p);
#ifdef DEBUG_MODE
LOG("exit ObjectManager::createPlayer");
#endif
	return p;
}

Monster* ObjectManager::createMonster(const Ogre::Vector3 position)
{
	Ogre::String name = Ogre::StringConverter::toString(++_countObject);
	Monster* m = new Monster(this, mSceneMgr->getRootSceneNode(), name);
	m->getNode()->setPosition(position);
	mObjects[name] = m;
	return m;
}

Block* ObjectManager::createBlock(const Ogre::Vector3 position) {
	Ogre::String name = Ogre::StringConverter::toString(++_countObject);
	Block* b = new Block(this, mSceneMgr->getRootSceneNode(), name, 3);
	b->getNode()->setPosition(position);
	mObjects[name] = b;
	mTerrain.addBlock(*b);
	return b;
}

void ObjectManager::loadScene() {
#ifdef DEBUG_MODE
LOG("enter ObjectManager::loadScene");
#endif
	std::vector<Ogre::Vector3> chunk = mMapManager.loadChunk(Vector3::ZERO);
	for(std::vector<Vector3>::iterator it = chunk.begin(); it != chunk.end(); ++it)
		createBlock(*it);
	mActiveObjects.push_back(createMonster(Vector3(3,1.5,5)));
#ifdef DEBUG_MODE
LOG("exit ObjectManager::loadScene");
#endif
}

Vector3 ObjectManager::getGravity(PhysicalObject* obj) const
{
	return Vector3::NEGATIVE_UNIT_Y * 9.8;
}

double ObjectManager::getStrench(PhysicalObject* obj) const
{
	if(obj->isOnGround())
		return 10;
	else
		return 0.1;
}

Terrain& ObjectManager::getTerrain() {
	return mTerrain;
}



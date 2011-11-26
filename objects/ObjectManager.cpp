/*
 * ObjectManager.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "ObjectManager.h"
#include <algorithm>

unsigned long ObjectManager::_countObject = 0;

Vector3 round(const Vector3 vector)
{
	return Vector3(round(vector.x),round(vector.y),round(vector.z));
}

Vector3 floor(const Vector3 vector)
{
	return Vector3(floor(vector.x),floor(vector.y),floor(vector.z));
}

ObjectManager::ObjectManager(Ogre::SceneManager* scnMgr) :
	mSceneMgr(scnMgr),
	mCollisionTools(scnMgr),
	mTerrain(this, scnMgr->createStaticGeometry("staticObjects")),
	mPhysicalClock(Clock(0.02)),
	mMapManager(10)
{}

ObjectManager::~ObjectManager()
{
	clear();
}

void ObjectManager::clear()
{
	// On détruit tout les objets référencés
	for(std::map<std::string,PhysicalObject*>::iterator it = mObjects.begin();
			it != mObjects.end(); ++it) {
		delete (it->second);
	}
	mObjects.clear();
	mActiveObjects.clear();
	//mTerrain.clear();
}

PhysicalObject* ObjectManager::getObject(const Ogre::String& name) const {
	if (mObjects.find(name) == mObjects.end())
		return NULL;
	return mObjects.at(name);
}

void ObjectManager::moveWithCollisions(PhysicalObject* &obj, const Ogre::Real deltaTime)
{
LOG("enter moveWithCollisions");
    // Calcul des nouvelles positions des objets.
    // On gère les éventuelles collisions
    if(obj->getSpeed() != Ogre::Vector3::ZERO && deltaTime != 0){
    	Ogre::Vector3 distance = obj->getSpeed() * deltaTime;
    	Ogre::Vector3 newDistance = handleCollision(obj,distance);
        obj->getNode()->translate(newDistance); //TODO: TS_WORLD ?
        obj->setSpeed(newDistance/deltaTime);
        obj->setCollisionCorrection(newDistance - distance);
    }
LOG("exit moveWithCollisions");
}

bool ObjectManager::objectReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, PhysicalObject* &target) {
	target = NULL;
	Ogre::Real dist = 0;
	Ogre::Vector3 pos;
	Ogre::Entity* entity;
	Ogre::Vector3 polygon_normal;

	// On cherche si un objet actif est atteint
	if (mCollisionTools.raycastFromPoint(from, normal, pos, entity, dist,polygon_normal)
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
	target = NULL;
Ogre::LogManager::getSingleton().logMessage("entering blockReached");
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
Ogre::LogManager::getSingleton().logMessage("exiting blockReached");
	return false;
}

bool intersect(Vector3 position,Vector3 &distance, Vector3 obstacle, Vector3 volume)
{
LOG("enter intersect");
	float t = 0;
	if(distance.y < 0)
	{
		t = ((obstacle + volume).y -position.y)/distance.y;
		if( t > 0 && t < 1 &&
				(position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
		{
//			distance.y = obstacle.y + volume.y - position.y;
			distance.y = 0;
LOG("exit intersect");
			return true;
		}

	}
	else if(distance.y > 0)
	{
		t = ((obstacle - volume).y -position.y)/distance.y;
		if( t > 0 && t < 1 &&
				(position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
		{
			distance.y = 0;
LOG("exit intersect");
			return true;
		}
	}
	if(distance.x < 0)
	{
		t = ((obstacle + volume).x -position.x)/distance.x;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
		{
			distance.x = 0;
LOG("exit intersect");
			return true;
		}

	}
	else if(distance.x > 0)
	{
		t = ((obstacle - volume).x -position.x)/distance.x;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
		{
			distance.x = 0;
LOG("exit intersect");
			return true;
		}
	}

	if(distance.z < 0)
	{
		t = ((obstacle + volume).z -position.z)/distance.z;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x)
		{
			distance.z = 0;
LOG("exit intersect");
			return true;
		}

	}
	else if(distance.z > 0)
	{
		t = ((obstacle - volume).z -position.z)/distance.z;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x)
		{
			distance.z = 0;
LOG("exit intersect");
			return true;
		}
	}
LOG("exit intersect");
	return false;
}

Ogre::Vector3 ObjectManager::handleCollision(const PhysicalObject* obj, Vector3 deplacement)
{
LOG("enter handleCollision");
	Ogre::Vector3 volume = obj->getVolume();
	for(int i = -1; i<=1; i += 2)
	{
		for(int j = -1; j<=1; j += 2)
		{
			for(int k = -1; k<=1; k+=2)
			{
				if( deplacement.x * i <=0 && deplacement.y * j <= 0 && deplacement.z * k <= 0)
					continue;
				Ogre::Vector3 point = obj->getNode()->getPosition() + Vector3(i*volume.x,j*volume.y,k*volume.z);
				Ogre::Vector3 planned_move = deplacement;
				do
				{
					planned_move = deplacement;
					if(!mTerrain.isFree(Triplet(round(point+Vector3(deplacement.x,0,0)))))
					{
						Ogre::String name = mTerrain.getBlock(Triplet(round(point+Vector3(deplacement.x,0,0))))->getName();
						PhysicalObject* obstacle = mObjects.find(name)->second;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
					if(!mTerrain.isFree(Triplet(round(point+Vector3(0,deplacement.y,0)))))
					{
						Ogre::String name = mTerrain.getBlock(Triplet(round(point+Vector3(0,deplacement.y,0))))->getName();
						PhysicalObject* obstacle = mObjects.find(name)->second;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
					if(!mTerrain.isFree(Triplet(round(point+Vector3(0,0,deplacement.z)))))
					{
						Ogre::String name = mTerrain.getBlock(Triplet(round(point+Vector3(0,0,deplacement.z))))->getName();
						PhysicalObject* obstacle = mObjects.find(name)->second;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
				} while(planned_move != deplacement);

			}
		}
	}
LOG("exit handleCollision");
	return deplacement;
}

void ObjectManager::gameOver()
{
LOG("GAME OVER");
//	clear();
//	exit(0);
}

void ObjectManager::updateObjects(Ogre::Real deltaTime) {
LOG("enter updateObjects");
LOG("nbr objets : " + StringConverter::toString(mObjects.size()) + ", actifs : " + StringConverter::toString(mActiveObjects.size()));
	PhysicalObject* obj = NULL;
	while(mPhysicalClock.ticked(deltaTime)){
LOG("tick");
		for(std::vector<PhysicalObject*>::iterator it = mActiveObjects.begin();
			it != mActiveObjects.end(); ++it) {
			obj = *it;

			// On lance d'abord les update personalisés de chaque objet
		// (pour les animations, modifications de vitesse, etc...).
LOG("pre object updated");
			obj->update(mPhysicalClock.getStep());
LOG("post object updated");
		// Calcul des nouvelles positions des objets.
			moveWithCollisions(obj, mPhysicalClock.getStep());
		}
	}
	for(std::vector<PhysicalObject*>::iterator it = mActiveObjects.begin(); it != mActiveObjects.end();++it)
	{
		obj = *it;
LOG("Object1 : " + obj->getName() + ", position : " + StringConverter::toString(obj->getNode()->getPosition()));
		if(obj->getId() == (ObjectId_t) 1)
		{
			PhysicalObject* obj2 = NULL;
			for(std::vector<PhysicalObject*>::iterator it2 = mActiveObjects.begin(); it2 != mActiveObjects.end();++it2)
			{
				obj2 = *it2;
LOG("Object2 : " + obj->getName() + ", position : " + StringConverter::toString(obj2->getNode()->getPosition()));
				if(obj != obj2)
				{
					if((obj2->getNode()->getPosition() - obj->getNode()->getPosition()).length() < 0.9)
					{
						gameOver();
					}
				}
			}
		}
	}
LOG("exit updateObjects");
}

Player* ObjectManager::createPlayer(Ogre::Camera* camera) {
	Ogre::String name = Ogre::StringConverter::toString(++_countObject);
	Player* p = new Player(this, name, camera);
	mObjects[name] = p;
	mActiveObjects.push_back(p);
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
LOG("enter loadScene");
	std::vector<Ogre::Vector3> chunk = mMapManager.loadChunk(Vector3::ZERO);
	for(std::vector<Vector3>::iterator it = chunk.begin(); it != chunk.end(); ++it)
		createBlock(*it);
	mActiveObjects.push_back(createMonster(Vector3(3,1.5,5)));
LOG("exit loadScene");
}

Vector3 ObjectManager::getGravity(PhysicalObject* obj) const
{
	return Vector3::NEGATIVE_UNIT_Y * 9.8;
}

double ObjectManager::getStrench(PhysicalObject* obj) const
{
	if(isOnGround(obj))
		return 10;
	else
		return 0.1;
}

Terrain& ObjectManager::getTerrain() {
	return mTerrain;
}

bool ObjectManager::isOnGround(PhysicalObject* obj) const
{
	Vector3 position = obj->getNode()->getPosition();

	if(mTerrain.isFree(Triplet(round(position.x),floor(position.y-obj->getVolume().y),round(position.z))))
		return false;
	PhysicalObject* obstacle = mTerrain.getBlock(Triplet(round(position.x),floor(position.y-obj->getVolume().y),round(position.z)));
	return ( 0.01 > position.y-obj->getVolume().y - obstacle->getNode()->getPosition().y - obstacle->getVolume().y);
}

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
	mTerrain(std::map<Triplet,String>()),
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
	mTerrain.clear();
}

void ObjectManager::moveWithCollisions(PhysicalObject* &obj, const Ogre::Real deltaTime)
{
    // Calcul des nouvelles positions des objets.
    // On gère les éventuelles collisions
    if(obj->getSpeed() != Ogre::Vector3::ZERO && deltaTime != 0){
    	Ogre::Vector3 distance = obj->getSpeed() * deltaTime;
    	handleCollision(obj,distance);
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
			return true;
		}
	}
	return false;
}

bool intersect(Vector3 position,Vector3 &distance, Vector3 obstacle, Vector3 volume)
{
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
			return true;
		}
	}

	return false;
}

void ObjectManager::handleCollision(const PhysicalObject* obj, Vector3 &deplacement)
{
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
					if(mTerrain.find(Triplet(round(point+Vector3(deplacement.x,0,0)))) != mTerrain.end())
					{
						Ogre::String name = mTerrain.find(Triplet(round(point+Vector3(deplacement.x,0,0))))->second;
						PhysicalObject* obstacle = mObjects.find(name)->second;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
					if(mTerrain.find(Triplet(round(point+Vector3(0,deplacement.y,0)))) != mTerrain.end())
					{
						Ogre::String name = mTerrain.find(Triplet(round(point+Vector3(0,deplacement.y,0))))->second;
						PhysicalObject* obstacle = mObjects.find(name)->second;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
					if(mTerrain.find(Triplet(round(point+Vector3(0,0,deplacement.z)))) != mTerrain.end())
					{
						Ogre::String name = mTerrain.find(Triplet(round(point+Vector3(0,0,deplacement.z))))->second;
						PhysicalObject* obstacle = mObjects.find(name)->second;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
				} while(planned_move != deplacement);

			}
		}
	}
}


void ObjectManager::updateObjects(Ogre::Real deltaTime) {
	PhysicalObject* obj = NULL;
	for(std::vector<PhysicalObject*>::iterator it = mActiveObjects.begin();
			it != mActiveObjects.end(); ++it) {
		obj = *it;
		while(mPhysicalClock.ticked(deltaTime)){


		// On lance d'abord les update personalisés de chaque objet
		// (pour les animations, modifications de vitesse, etc...).
			obj->update(mPhysicalClock.getStep());

		// Calcul des nouvelles positions des objets.
			moveWithCollisions(obj, mPhysicalClock.getStep());
		}
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
	mTerrain[Triplet(position)] = name;
	//TODO: setInitialState ?
	mObjects[name] = b;
	return b;
}

void ObjectManager::loadScene() {
	std::vector<Ogre::Vector3> chunk = mMapManager.loadChunk(Vector3::ZERO);
	for(std::vector<Vector3>::iterator it = chunk.begin(); it != chunk.end(); ++it)
		createBlock(*it);
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

bool ObjectManager::isOnGround(PhysicalObject* obj) const
{
	Vector3 position = obj->getNode()->getPosition();

	if(mTerrain.find(Triplet(round(position.x),floor(position.y-obj->getVolume().y),round(position.z))) == mTerrain.end())
		return false;
	PhysicalObject* obstacle = mObjects.find(mTerrain.find(Triplet(round(position.x),floor(position.y-obj->getVolume().y),round(position.z)))->second)->second;
	return ( 0.01 > position.y-obj->getVolume().y - obstacle->getNode()->getPosition().y - obstacle->getVolume().y);
}

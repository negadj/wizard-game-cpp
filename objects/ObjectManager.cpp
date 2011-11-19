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
	mPhysicalClock(Clock(0.02))
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

void intersect(Vector3 position,Vector3 &distance, Vector3 obstacle, Vector3 volume)
{
	float t = 0;
	if(distance.x < 0)
	{
		t = ((obstacle + volume).x -position.x)/distance.x;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
//			distance.x = obstacle.x + volume.x - position.x;
			distance.x = 0;

	}
	else if(distance.x > 0)
	{
		t = ((obstacle - volume).x -position.x)/distance.x;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
//			distance.x = obstacle.x - volume.x - position.x;
			distance.x = 0;
	}
	if(distance.y < 0)
	{
		t = ((obstacle + volume).y -position.y)/distance.y;
		if( t > 0 && t < 1 &&
				(position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
//			distance.y = obstacle.y + volume.y - position.y;
			distance.y = 0;

	}
	else if(distance.y > 0)
	{
		t = ((obstacle - volume).y -position.y)/distance.y;
		if( t > 0 && t < 1 &&
				(position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x
				&& (position + t*distance).z > (obstacle - volume).z && (position + t*distance).z < (obstacle + volume).z)
//			distance.y = obstacle.y - volume.y - position.y;
			distance.y = 0;
	}
	if(distance.z < 0)
	{
		t = ((obstacle + volume).z -position.z)/distance.z;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x)
//			distance.z = obstacle.z + volume.z - position.z;
			distance.z = 0;

	}
	else if(distance.z > 0)
	{
		t = ((obstacle - volume).z -position.z)/distance.z;
		if( t > 0 && t < 1 &&
				(position + t*distance).y > (obstacle - volume).y && (position + t*distance).y < (obstacle + volume).y
				&& (position + t*distance).x > (obstacle - volume).x && (position + t*distance).x < (obstacle + volume).x)
//			distance.z = obstacle.z - volume.z - position.z;
			distance.z = 0;
	}
}

void ObjectManager::handleCollision(const PhysicalObject* obj, Vector3 &deplacement)
{
	Ogre::Vector3 volume = obj->getVolume();
	for(int i = -1; i<=1; i += 2)
	{
		for(int j = -1; j<=-1; j += 2)
		{
			for(int k = -1; k<=1; k+=2)
			{
				if( deplacement.x * i <=0 && deplacement.y * j <= 0 && deplacement.z * k <= 0)
					continue;
				Ogre::Vector3 point = obj->getNode()->getPosition() + Vector3(i*volume.x,j*volume.y,k*volume.z);
				if(mTerrain.find(Triplet(round(point + deplacement))) != mTerrain.end())
				{
					Ogre::String name = mTerrain.find(Triplet(round(point+deplacement)))->second;
					PhysicalObject* obstacle = mObjects.find(name)->second;
					intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());

				}

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
	for (int i=0; i<2; i++) {
		for (int j=0; j<2; j++) {

			createBlock(Vector3(2*i,-1,-2*j));
			createBlock(Vector3(2*i,-1,-2*j-1));
			createBlock(Vector3(2*i+1,-1,-2*j));
			createBlock(Vector3(2*i+1,-1,-2*j-1));
			for (int k=0; k<1; k++) {
				if (rand()%2 == 0) {
					createBlock(Vector3(i,k,-j)*2);
				}
			}
		}
	}
	return;
}

Vector3 ObjectManager::getGravity(Vector3 position) const
{
	return Vector3::NEGATIVE_UNIT_Y * 0;
}

double ObjectManager::getStrench(Vector3 position) const
{
	if(isOnGround(position))
		return 10;
	else
		return 0.1;
}

bool ObjectManager::isOnGround(Vector3 position) const
{
	return mTerrain.find(Triplet(round(position.x),position.y-1,round(position.z))) != mTerrain.end();
}

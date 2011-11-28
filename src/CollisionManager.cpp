/*
 * CollisionManager.cpp
 *
 *  Created on: 28 nov. 2011
 *      Author: gecko
 */

#include "CollisionManager.h"
#include "Triplet.h"
#include "ObjectManager.h"

bool intersect(Ogre::Vector3 position, Ogre::Vector3 &distance, Ogre::Vector3 obstacle, Ogre::Vector3 volume)
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

CollisionManager::CollisionManager(ObjectManager* objMgr, Ogre::SceneManager* scnMgr) :
	mObjMgr(objMgr),
	mCollisionTools(scnMgr)
{}

CollisionManager::~CollisionManager() {}

bool CollisionManager::raycast(const Ogre::Vector3 &point, const Ogre::Vector3 &normal, Ogre::Vector3 &result, Ogre::Entity* &target,
		float &closest_distance, Ogre::Vector3 &polygon_normal, const Ogre::Entity* source, const Ogre::uint32 queryMask) {
	return mCollisionTools.raycastFromPoint(point, normal, result, target, closest_distance, polygon_normal, source, queryMask);
}

void CollisionManager::moveWithCollisions(PhysicalObject* &obj, const Ogre::Real deltaTime)
{
    // Calcul des nouvelles positions des objets.
    // On gère les éventuelles collisions
    if(obj->getSpeed() != Ogre::Vector3::ZERO && deltaTime != 0){
    	Ogre::Vector3 distance = obj->getSpeed() * deltaTime;
    	Ogre::Vector3 newDistance = handleCollision(obj,distance);
        obj->getNode()->translate(newDistance); //TODO: TS_WORLD ?
        obj->setSpeed(newDistance/deltaTime);
        obj->setCollisionCorrection(newDistance - distance);
    }
}

Ogre::Vector3 CollisionManager::handleCollision(const PhysicalObject* obj, Ogre::Vector3 deplacement)
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
				Ogre::Vector3 point = obj->getNode()->getPosition() + Ogre::Vector3(i*volume.x,j*volume.y,k*volume.z);
				Ogre::Vector3 planned_move = deplacement;
				do
				{
					planned_move = deplacement;
					if(!mObjMgr->getTerrain().isFree(Triplet(round(point + Ogre::Vector3(deplacement.x,0,0)))))
					{
						Ogre::String name = mObjMgr->getTerrain().getBlock(Triplet(round(point + Ogre::Vector3(deplacement.x,0,0))))->getName();
						PhysicalObject* obstacle = mObjMgr->getObject(name);;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
					if(!mObjMgr->getTerrain().isFree(Triplet(round(point + Ogre::Vector3(0,deplacement.y,0)))))
					{
						Ogre::String name = mObjMgr->getTerrain().getBlock(Triplet(round(point + Ogre::Vector3(0,deplacement.y,0))))->getName();
						PhysicalObject* obstacle = mObjMgr->getObject(name);
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
					if(!mObjMgr->getTerrain().isFree(Triplet(round(point + Ogre::Vector3(0,0,deplacement.z)))))
					{
						Ogre::String name = mObjMgr->getTerrain().getBlock(Triplet(round(point + Ogre::Vector3(0,0,deplacement.z))))->getName();
						PhysicalObject* obstacle = mObjMgr->getObject(name);;
						intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
					}
				} while(planned_move != deplacement);

			}
		}
	}
	return deplacement;
}

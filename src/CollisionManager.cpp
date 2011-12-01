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
#ifdef DEBUG_MODE
LOG("enter Collision::moveWitCollisions");
#endif
    // Calcul des nouvelles positions des objets.
    // On gère les éventuelles collisions
    if(obj->getSpeed() != Ogre::Vector3::ZERO && deltaTime != 0){
    	Ogre::Vector3 distance = obj->getSpeed() * deltaTime;
    	Ogre::Vector3 newDistance = handleCollision(obj,distance);
        obj->getNode()->translate(newDistance); //TODO: TS_WORLD ?
        /*
         * UNe fois les collisons gérées, on met à jour la vitesse et on renseigne la correction
         */
        obj->setSpeed(newDistance/deltaTime);
        obj->setCollisionCorrection(newDistance - distance);
    }
#ifdef DEBUG_MODE
LOG("exit Collision::moveWitCollisions");
#endif
}

Ogre::Vector3 CollisionManager::handleCollision(const PhysicalObject* obj, Ogre::Vector3 deplacement)
{
#ifdef DEBUG_MODE
LOG("enter Collision::handleCollision");
#endif
	/*
	 * On récupère tout les points du parallepipede et on verifie que le mouvement ne pose de probleme pour aucune
	 * TODO : Dans le cas d'objets très grand (aucun pour l'instant), il vaudrait peut être mieux d'ajouter des points intermédiaires
	 */
	Ogre::Vector3 volume = obj->getVolume();
	for(int i = -1; i<=1; i += 2)
	{
		for(int j = -1; j<=1; j += 2)
		{
			for(int k = -1; k<=1; k+=2)
			{
				/* Si le point se re trouve à l'oppose du deplacement, pas la peine de s'en occcuper */
				if( deplacement.x * i <=0 && deplacement.y * j <= 0 && deplacement.z * k <= 0)
					continue;
				Ogre::Vector3 point = obj->getNode()->getPosition() + Ogre::Vector3(i*volume.x,j*volume.y,k*volume.z);
				Ogre::Vector3 planned_move = deplacement;
				do
				{
					/*
					 * On décompose le déplacment dans les 3 directions et on regarde les collisions pour chacun
					 */
					std::vector<Ogre::Vector3> moveComposant = std::vector<Ogre::Vector3>();
					moveComposant.push_back(Ogre::Vector3(deplacement.x,0,0));
					moveComposant.push_back(Ogre::Vector3(0,deplacement.y,0));
					moveComposant.push_back(Ogre::Vector3(0,0,deplacement.z));
					moveComposant.push_back(Ogre::Vector3(deplacement.x,0,deplacement.z));
					moveComposant.push_back(Ogre::Vector3(0,deplacement.y,deplacement.z));
					moveComposant.push_back(Ogre::Vector3(0,deplacement.y,deplacement.z));
					moveComposant.push_back(Ogre::Vector3(deplacement.x,deplacement.y,deplacement.z));
					planned_move = deplacement;
					for(std::vector<Ogre::Vector3>::iterator it = moveComposant.begin() ; it < moveComposant.end() ; ++it)
					{
						if(!mObjMgr->isBlockFree(Triplet(round(point + *it))))
						{
							Ogre::String name = mObjMgr->getBlock(Triplet(round(point + *it)))->getName();
							PhysicalObject* obstacle = mObjMgr->getObject(name);;
							intersect(point,deplacement,obstacle->getNode()->getPosition(),obstacle->getVolume());
						}
					}
					/*
					 * On itère jusqu'a ce qu'il ny ait plus de correction
					 */
				} while(planned_move != deplacement);

			}
		}
	}
#ifdef DEBUG_MODE
LOG("exit Collision::handleCollision");
#endif
	return deplacement;
}

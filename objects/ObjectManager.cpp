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
	mTerrain(std::map<Triplet,String>())
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
			std::cout << target->getName() << " " << entity->getParentSceneNode()->getName() << std::endl;
			return true;
		}
	}
	return false;
}

std::vector<Ogre::Vector3> getContactSurface(const Ogre::Vector3 position, const Ogre::Vector3 volume, const Ogre::Vector3 normal)
{
	std::vector<Ogre::Vector3> result;
	if(normal == Vector3::UNIT_X || normal == Vector3::NEGATIVE_UNIT_X)
	{
		int i, jmax, kmax;
		if(normal == Vector3::UNIT_X)
			i = volume.x - 1;
		else
			i = 0;
		if(round(position.y) == position.y)
			jmax = volume.y;
		else
			jmax = volume.y + 1;
		if(round(position.z) == position.z)
			kmax = volume.z;
		else
			kmax = volume.z + 1;
		for(int j = 0; j < jmax; ++j)
			for(int k = 0; k < kmax; k++)
				result.push_back(Vector3(i,j,k));
	}
	else if(normal == Vector3::UNIT_Y|| normal == Vector3::NEGATIVE_UNIT_Y)
	{
		int imax, j, kmax;
		if(normal == Vector3::UNIT_Y)
			j = volume.y - 1;
		else
			j = 0;
		if(round(position.x) == position.x)
			imax = volume.x;
		else
			imax = volume.x + 1;
		if(round(position.z) == position.z)
			kmax = volume.z;
		else
			kmax = volume.z + 1;
		for(int i = 0; i < imax; ++i)
			for(int k = 0; k < kmax; k++)
				result.push_back(Vector3(i,j,k));
	}
	else if(normal == Vector3::UNIT_Z|| normal == Vector3::NEGATIVE_UNIT_Z)
	{
		int imax, jmax, k;
		if(normal == Vector3::UNIT_Z)
			k = volume.z - 1;
		else
			k = 0;
		if(round(position.x) == position.x)
			imax = volume.x;
		else
			imax = volume.x + 1;
		if(round(position.y) == position.y)
			jmax = volume.z;
		else
			jmax = volume.z + 1;
		for(int i = 0; i < imax; ++i)
			for(int j = 0; j < jmax; j++)
				result.push_back(Vector3(i,j,k));
	}
	return result;

}

void ObjectManager::handleCollision(const PhysicalObject* obj, Vector3 &deplacement)
{
	std::vector<Ogre::Vector3> grid ;

	// Collision selon x
	double dx =  deplacement.dotProduct(Vector3::UNIT_X);
	if (dx > 0)
	{
		grid = getContactSurface(obj->getNode()->getPosition(),obj->getVolume(),Vector3::UNIT_X);
		for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
		{
			double x1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X);
			std::cout << *it << " ";
			for(int i = floor(x1) ; i <= floor(x1 + dx); ++i )
			{
				if(mTerrain.find(Triplet(i+1,floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Y)),floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Z)))) != mTerrain.end())
				{
					dx = std::min(dx,i - x1);
					break;
				}
			}

		}
		std::cout << std::endl;
	}
	else if(dx < 0)
	{
		grid = getContactSurface(obj->getNode()->getPosition(),obj->getVolume(),Vector3::NEGATIVE_UNIT_X);
		for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
		{
			double x1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X);
			for(int i = floor(x1)+1 ; i >= floor(x1 + dx)+1; --i )
			{
				if(mTerrain.find(Triplet(i-1,floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Y)),floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Z)))) != mTerrain.end())
				{
					dx = std::max(dx,i - x1);
					break;
				}
			}

		}
	}

	deplacement.x = dx;

	// Deplacement selon y
	double dy =  deplacement.dotProduct(Vector3::UNIT_Y);
	if (dy > 0)
	{
		grid = getContactSurface(obj->getNode()->getPosition(),obj->getVolume(),Vector3::UNIT_Y);
		for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
		{
			double y1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Y);
			std::cout << *it << " ";
			for(int j = floor(y1) ; j <= floor(y1 + dy); ++j )
			{
				if(mTerrain.find(Triplet(floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X)),j+1,floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Z)))) != mTerrain.end())
				{
					dy = std::min(dy,j - y1);
					break;
				}
			}

		}
		std::cout << std::endl;
	}
	else if(dy < 0)
	{
		grid = getContactSurface(obj->getNode()->getPosition(),obj->getVolume(),Vector3::NEGATIVE_UNIT_Y);
		for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
		{
			double y1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Y);
			for(int j = floor(y1)+1 ; j >= floor(y1 + dy)+1; --j )
			{
				if(mTerrain.find(Triplet(floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X)),j - 1,floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Z)))) != mTerrain.end())
				{
					dy = std::max(dy,j - y1);
					break;
				}
			}

		}
	}

	deplacement.y = dy;

	// Deplacement selon z
		double dz =  deplacement.dotProduct(Vector3::UNIT_Z);
		if (dz > 0)
		{
			grid = getContactSurface(obj->getNode()->getPosition(),obj->getVolume(),Vector3::UNIT_Z);
			for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
			{
				double z1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Z);
				std::cout << *it << " ";
				for(int k = floor(z1) ; k <= floor(z1 + dz); ++k )
				{
					if(mTerrain.find(Triplet(floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X)),floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Y)),k+1)) != mTerrain.end())
					{
						dz = std::min(dz,k - z1) ;
						break;
					}
				}

			}
			std::cout << std::endl;
		}
		else if(dz < 0)
		{
			grid = getContactSurface(obj->getNode()->getPosition(),obj->getVolume(),Vector3::NEGATIVE_UNIT_Z);
			for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
			{
				double z1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Z);
				for(int k = floor(z1)+1 ; k >= floor(z1 + dz)+1; --k )
				{
					if(mTerrain.find(Triplet(floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X)),floor((obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_Y)),k-1)) != mTerrain.end())
					{
						dz = std::max(dz,k - z1);
						break;
					}
				}

			}
		}

		deplacement.z = dz;
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
	mTerrain[Triplet(position)] = name;
	//TODO: setInitialState ?
	mObjects[name] = b;
	return b;
}

void ObjectManager::loadScene() {
	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			for (int k=0; k<2; k++) {
				if (rand()%3 == 0) {
					createBlock(Vector3(i,k,-j)*2);
				}
			}
		}
	}
	return;
}




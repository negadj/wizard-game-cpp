/*
 * ObjectManager.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "ObjectManager.h"

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

    	//const std::vector<std::pair<Ogre::Vector3,Ogre::Vector3> > skeleton = obj->getSkeleton();

//        for(std::vector<std::pair<Ogre::Vector3,Ogre::Vector3> >::const_iterator it = skeleton.begin(); it != skeleton.end(); ++it)
//        {
//        	if(it->second.x*distance.x <= 0 && it->second.y*distance.y <= 0 && it->second.z*distance.z <= 0 )
//        		continue;
//      	Ogre::Vector3 point = obj->getNode()->getPosition();//+ it->first;
//        	int counter =0;
//            float dist = 0.0f;
//            Ogre::Vector3 destination = Ogre::Vector3::ZERO;
//            Ogre::MovableObject *target;
//            Ogre::Vector3 polygon_normal;
//        	while(mCollisionTools.raycastFromPoint( point, distance.normalisedCopy(), destination, target, dist, polygon_normal, obj->getEntity(), 0xFFFFFFFF)
//        			&& dist < distance.length())
//        	{
//        		//std::cout <<target->getName() << " " << it->first << " " << counter << std::endl;
//        	// TODO: Parfois, la boucle se lock. Un compteur permet d'éviter un freeze complet
//        		if (counter > 10)
//        			distance = Ogre::Vector3::ZERO;
//        		if(distance.dotProduct(polygon_normal) < 0)
//        			polygon_normal = -polygon_normal;
//        		distance -= polygon_normal.dotProduct(distance - distance.normalisedCopy() * dist) * polygon_normal; //destination);
//        		counter+=1;
//        	}
////        }
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
			i = volume.x;
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
		for(int j = 0; j <= jmax; ++j)
			for(int k = 0; k <= kmax; k++)
				result.push_back(Vector3(i,j,k));
	}

	return result;

}

void ObjectManager::handleCollision(const PhysicalObject* obj, Vector3 &deplacement)
{
	std::vector<Ogre::Vector3> grid = std::vector<Ogre::Vector3>();
	Ogre::Vector3 volume = obj->getVolume();
	for(int i = 0; i < volume.x; ++i)
	{
		for(int j = 0; j < volume.y; ++j)
		{
			for(int k = 0; k < volume.z; ++k)
			{
				grid.push_back(Vector3(i,j,k));
			}
		}
	}
	// Collision selon x
	double dx =  deplacement.dotProduct(Vector3::UNIT_X);
	if (dx > 0)
	{
		for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
		{
			double x1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X);
			std::cout << x1 << " ";
			for(int i = floor(x1) ; i <= floor(x1 + dx); ++i )
			{
				if(mTerrain.find(Triplet(i+1,0,round(obj->getNode()->getPosition().dotProduct(Vector3::UNIT_Z)))) != mTerrain.end())
				{
					dx = i - x1 ;
					break;
				}
			}

		}
		std::cout << std::endl;
	}
	else if(dx < 0)
	{
		for(std::vector<Vector3>::const_iterator it = grid.begin(); it != grid.end(); ++it)
		{
			double x1 = (obj->getNode()->getPosition() + *it).dotProduct(Vector3::UNIT_X);
			for(int i = floor(x1)+1 ; i >= floor(x1 + dx)+1; --i )
			{
				if(mTerrain.find(Triplet(i-1,
										0,
										round(obj->getNode()->getPosition().dotProduct(Vector3::UNIT_Z))
								       )
								 ) != mTerrain.end())
				{
					dx = i - x1;
					break;
				}
			}

		}
	}

	deplacement.x = dx;
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
		for (int j=0; j<1; j++) {
			for (int k=0; k<1; k++) {
				if (true) {
					createBlock(Vector3(i,k,-j) * 3);
				}
			}
		}
	}
	return;
}




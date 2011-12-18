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
	mCollisionMgr(this, scnMgr),
	mObjects(std::map<std::string,PhysicalObject*>()),
	mActiveObjects(std::set<PhysicalObject*>()),
	mObjectsToDelete(std::queue<Ogre::String>()),
	mTerrain(this, scnMgr->createStaticGeometry("terrain")),
	mPhysicalClock(Clock(0.02)),
	mMonsterClock(Clock(5)),
	mMapManager(10)
{}

ObjectManager::~ObjectManager()
{
#ifdef DEBUG_MODE
LOG("enter ObjectManager destructor");
#endif

	// On détruit tous les objets
	for(std::map<std::string,PhysicalObject*>::iterator it = mObjects.begin();
			it != mObjects.end(); ++it) {
		delete (it->second);
	}
#ifdef DEBUG_MODE
LOG("objects deleted");
#endif

	// On nettoie la scène
	mSceneMgr->clearScene();
#ifdef DEBUG_MODE
LOG("scene cleared");
#endif

	mActiveObjects.clear();
	mObjects.clear();
#ifdef DEBUG_MODE
LOG("exit ObjectManager destructor");
#endif
}

void ObjectManager::setActive(PhysicalObject* const obj, bool active) {
	//Pas de checks d'existence/doublons nécessaires, c'est un set.
	if (active) {
		mActiveObjects.insert(obj);
	}
	else {
		mActiveObjects.erase(obj);
	}
}

bool ObjectManager::isActive(PhysicalObject* const obj) const {
	return (mActiveObjects.find(obj) != mActiveObjects.end());
}

PhysicalObject* ObjectManager::getObject(const Ogre::String& name) {
//#ifdef DEBUG_MODE
//LOG("call ObjectManager::getObject");
//#endif
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

bool ObjectManager::blockReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, Block* &target, Ogre::Vector3* faceVector) {
#ifdef DEBUG_MODE
LOG("call ObjectManager::blockReached");
#endif
	target = NULL;
	// On trouve le premier bloc libre sur la trajectoire
	// en regardant régulièrement le long de la direction visée.
	// TODO: trouver un meilleur algo
	Ogre::Vector3 currentPos;
	int i = int (from.x), j = int (from.y), k = int (from.z);
	int faceI = 0, faceJ = 0, faceK = 0;
	for (int step = 0; step < int(reachRadius*10); step++) {
		currentPos = from + (double(step) / 10) * normal;
		if (i != int(currentPos.x + 0.5) || j != int(currentPos.y + 0.5) || k != int(currentPos.z + 0.5)) {
		    faceI=i-int(currentPos.x + 0.5); //returne -1 0 ou 1
		    faceJ=j-int(currentPos.y + 0.5); // afin de savoir quelle face a changé.
		    faceK=k-int(currentPos.z + 0.5);
			i = int(currentPos.x + 0.5);
			j = int(currentPos.y + 0.5);
			k = int(currentPos.z + 0.5);
			if (!mTerrain.isFree(Triplet(i,j,k))) {
				target = mTerrain.getBlock(Triplet(i,j,k));
				if (faceVector)
                 {*faceVector=Ogre::Vector3(faceI,faceJ,faceK);}
				return true;
			}
		}
	}
	return false;
}

void ObjectManager::updateObjects(Ogre::Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter ObjectManager::updateObjects");
LOG("nbr d'objets : " + Ogre::StringConverter::toString(mObjects.size()) + ", actifs : " + Ogre::StringConverter::toString(mActiveObjects.size()) + ", a detruire : " + Ogre::StringConverter::toString(mObjectsToDelete.size()));
#endif

	// On supprime les objets qui doivent disparaîtres.
	while (!mObjectsToDelete.empty()) {
#ifdef DEBUG_MODE
LOG("ObjectsToDelete non vide, front = object n° " + mObjectsToDelete.front());
#endif
		removeObject(mObjects[mObjectsToDelete.front()]);
#ifdef DEBUG_MODE
LOG("object deleted");
#endif
		mObjectsToDelete.pop();
	}

#ifdef DEBUG_MODE
LOG("nbr d'objets : " + Ogre::StringConverter::toString(mObjects.size()) + ", actifs : " + Ogre::StringConverter::toString(mActiveObjects.size()));
#endif

    // On fait "poper" des monstres régulièrement

	PhysicalObject* obj = NULL;
	Ogre::Real monsterTime = deltaTime; // Copie nécessaire pour ne pas modifier deltaTime
	if(mMonsterClock.ticked(monsterTime))
	{
		createMonster(Vector3(rand()%20,20,rand()%20));
	}
	while(mPhysicalClock.ticked(deltaTime)){
#ifdef DEBUG_MODE
LOG("tick");
#endif
		for(std::set<PhysicalObject*>::iterator it = mActiveObjects.begin();
			it != mActiveObjects.end(); ++it) {
			obj = *it;

			// On lance d'abord les update personalisés de chaque objet
			obj->preCollisionUpdate(mPhysicalClock.getStep());
		// Calcul des nouvelles positions des objets.
			mCollisionMgr.moveWithCollisions(obj, mPhysicalClock.getStep());
			obj->postCollisionUpdate(mPhysicalClock.getStep());
			if(obj->getNode()->getPosition().y < -10)
				obj->setIntegrity(0);
		}
	}
	for(std::set<PhysicalObject*>::iterator it = mActiveObjects.begin(); it != mActiveObjects.end();++it)
	{
		obj = *it;
		if(obj->getObjectType() == TYPE_FRIENDLY)
		{
			PhysicalObject* obj2 = NULL;
			for(std::set<PhysicalObject*>::iterator it2 = mActiveObjects.begin(); it2 != mActiveObjects.end();++it2)
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

Ogre::String ObjectManager::getUniqueName() {
	return Ogre::StringConverter::toString(++_countObject);
}

void ObjectManager::registerObject(PhysicalObject* object) {
	mObjects[object->getName()] = object;
//	if (active)
//		mActiveObjects.insert(object);
//	object->addListener(this);
}

Player* ObjectManager::createPlayer(Ogre::Camera* camera) {
#ifdef DEBUG_MODE
LOG("enter ObjectManager::createPlayer");
#endif

	Player* p = new Player(this, getUniqueName(), camera);
	registerObject(p);

#ifdef DEBUG_MODE
LOG("exit ObjectManager::createPlayer");
#endif
	return p;
}

Monster* ObjectManager::createMonster(const Ogre::Vector3 position)
{
	Ogre::Vector3 realPosition = position;
	std::vector<Vector3> correction = std::vector<Vector3>();
	correction.push_back(Ogre::Vector3::UNIT_X);
	correction.push_back(Ogre::Vector3::UNIT_Z);
	correction.push_back(Ogre::Vector3::NEGATIVE_UNIT_X);
	correction.push_back(Ogre::Vector3::NEGATIVE_UNIT_Z);
	std::vector<Vector3>::iterator it = correction.begin();
	while(!mTerrain.isFree(round(realPosition)))
	{
		realPosition = position + *it;
		++it;
	}
	Monster* m = new Monster(this, mSceneMgr->getRootSceneNode(), getUniqueName());
	registerObject(m);
	m->getNode()->setPosition(position);

	return m;
}

Block* ObjectManager::createBlock(const Triplet& position, bool add) {
	if (!isBlockFree(position))
		return NULL;
	Block* b = new Block(this, mSceneMgr->getRootSceneNode(), getUniqueName());
	registerObject(b);
	b->getNode()->setPosition(position);
	if(add)
		mTerrain.addBlock(*b);

	return b;
}

void ObjectManager::loadScene() {
#ifdef DEBUG_MODE
LOG("enter ObjectManager::loadScene");
#endif
	std::vector<Ogre::Vector3> chunk = mMapManager.loadChunk(Vector3::ZERO);
	std::vector<Block*> blocks= std::vector<Block*>();
	for(std::vector<Vector3>::iterator it = chunk.begin(); it != chunk.end(); ++it)
	{
		blocks.push_back(createBlock(*it, false));
	}
	mTerrain.addBlocks(blocks);
	createMonster(Vector3(rand()%18 + 1,1.5,rand()%18 + 1));
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

Block* ObjectManager::getBlock(const Triplet& pos) {
	return mTerrain.getBlock(pos);
}

bool ObjectManager::isBlockFree(const Triplet& pos) const {
	return mTerrain.isFree(pos);
}

void ObjectManager::objectDestroyed(const PhysicalObject* object) {
//	if (object->mRemoveOnDestroy)
//		requestRemoval(object->getName());
}

void ObjectManager::requestDestruction(Ogre::String name) {
	mObjectsToDelete.push(name);
}

void ObjectManager::removeObject(PhysicalObject* object) {
#ifdef DEBUG_MODE
LOG("enter ObjectManager::removeObject");
#endif
#ifdef DEBUG_MODE
LOG("object n° : " + object->getName());
#endif
	if (object->getObjectType() == TYPE_BLOCK) {
		mTerrain.removeBlock(object->getNode()->getPosition());
	}
	mObjects.erase(object->getName());
	if (mActiveObjects.find(object) != mActiveObjects.end())
		mActiveObjects.erase(mActiveObjects.find(object));
	delete object;
#ifdef DEBUG_MODE
LOG("exit ObjectManager::removeObject");
#endif
}

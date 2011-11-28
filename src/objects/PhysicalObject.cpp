/*
 * PhysicalObject.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "PhysicalObject.h"
#include "../ObjectManager.h"

PhysicalObject::PhysicalObject(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, int id, Ogre::String meshName,Ogre::Vector3 volume, Ogre::String description) :
	mObjectManager(objectManager),
	mId(id),
	mName(name),
	mDescription(description),
	mNode(originNode->createChildSceneNode(name)),
	mEntity(0),
	mAcceleration(Ogre::Vector3::ZERO),
	mSpeed(Ogre::Vector3::ZERO),
	mSolidity(20),
	mDensity(1),
	mIntegrity(100),
	mVolume(volume),
	mCollisionCorrection(Ogre::Vector3::ZERO),
	mListeners(std::set<PhysicalObjectListener*>())
{
	mEntity = mNode->getCreator()->createEntity(mName, meshName);
	mNode->attachObject(mEntity);
}

PhysicalObject::~PhysicalObject() {}

void PhysicalObject::update(Ogre::Real deltaTime) {}

Ogre::Vector3 PhysicalObject::getAcceleration() const
{
    return mAcceleration;
}

ObjectId_t PhysicalObject::getId() const
{
    return mId;
}

int PhysicalObject::getIntegrity() const
{
    return mIntegrity;
}

std::string PhysicalObject::getName() const
{
    return mName;
}

Ogre::SceneNode* PhysicalObject::getNode() const
{
    return mNode;
}

Ogre::Vector3 PhysicalObject::getSpeed() const
{
    return mSpeed;
}

void PhysicalObject::setAcceleration(Ogre::Vector3 acceleration)
{
    this->mAcceleration = mAcceleration;
}

void PhysicalObject::setIntegrity(int integrity)
{
	if (integrity <= 0) {
		this->mIntegrity = 0;
		destroy();
		fireDestruction();
	}
	else
		this->mIntegrity = integrity;
}

float PhysicalObject::getDensity() const
{
    return mDensity;
}

int PhysicalObject::getSolidity() const
{
    return mSolidity;
}

void PhysicalObject::setDensity(float density)
{
    this->mDensity = density;
}

void PhysicalObject::setEntity(Ogre::Entity *entity)
{
    this->mEntity = entity;
}

Ogre::String PhysicalObject::getDescription() const
{
    return mDescription;
}

ObjectManager* PhysicalObject::getObjectManager() const
{
    return mObjectManager;
}

void PhysicalObject::setDescription(Ogre::String description)
{
    this->mDescription = description;
}

Ogre::Entity *PhysicalObject::getEntity() const
{
    return mEntity;
}

void PhysicalObject::setSolidity(int solidity)
{
    this->mSolidity = solidity;
}

void PhysicalObject::setSpeed(Ogre::Vector3 speed)
{
    this->mSpeed = speed;
}

void PhysicalObject::addSpeed(Ogre::Vector3 speed)
{
    this->mSpeed += speed;
}

const Ogre::Vector3  PhysicalObject::getVolume() const
{
	return mVolume;
}

void PhysicalObject::destroy() {}

std::vector<Ogre::Vector3> PhysicalObject::getContactSurface(const Ogre::Vector3 normal) const
{
	std::vector<Ogre::Vector3> result;
	Vector3 position = getNode()->getPosition();
	if(normal == Vector3::UNIT_X || normal == Vector3::NEGATIVE_UNIT_X)
	{
		int i, jmax, kmax;
		if(normal == Vector3::UNIT_X)
			i = mVolume.x - 1;
		else
			i = 0;
		if(round(position.y) == position.y)
			jmax = mVolume.y;
		else
			jmax = mVolume.y + 1;
		if(round(position.z) == position.z)
			kmax = mVolume.z;
		else
			kmax = mVolume.z + 1;
		for(int j = 0; j < jmax; ++j)
			for(int k = 0; k < kmax; k++)
				result.push_back(Vector3(i,j,k));
	}
	else if(normal == Vector3::UNIT_Y|| normal == Vector3::NEGATIVE_UNIT_Y)
	{
		int imax, j, kmax;
		if(normal == Vector3::UNIT_Y)
			j = mVolume.y - 1;
		else
			j = 0;
		if(round(position.x) == position.x)
			imax = mVolume.x;
		else
			imax = mVolume.x + 1;
		if(round(position.z) == position.z)
			kmax = mVolume.z;
		else
			kmax = mVolume.z + 1;
		for(int i = 0; i < imax; ++i)
			for(int k = 0; k < kmax; k++)
				result.push_back(Vector3(i,j,k));
	}
	else if(normal == Vector3::UNIT_Z|| normal == Vector3::NEGATIVE_UNIT_Z)
	{
		int imax, jmax, k;
		if(normal == Vector3::UNIT_Z)
			k = mVolume.z - 1;
		else
			k = 0;
		if(round(position.x) == position.x)
			imax = mVolume.x;
		else
			imax = mVolume.x + 1;
		if(round(position.y) == position.y)
			jmax = mVolume.z;
		else
			jmax = mVolume.z + 1;
		for(int i = 0; i < imax; ++i)
			for(int j = 0; j < jmax; j++)
				result.push_back(Vector3(i,j,k));
	}
	return result;
}

Ogre::Vector3 PhysicalObject::getCollisionCorrection() const
{
	return mCollisionCorrection;
}

void PhysicalObject::setCollisionCorrection(Ogre::Vector3 correction)
{
	mCollisionCorrection = correction;
}

bool PhysicalObject::isOnGround() const
{
	Ogre::Vector3 position = getNode()->getPosition();

	if(mObjectManager->getTerrain().isFree(Triplet(round(position.x),floor(position.y - getVolume().y),round(position.z))))
		return false;
	PhysicalObject* obstacle = mObjectManager->getTerrain().getBlock(Triplet(round(position.x),floor(position.y - getVolume().y),round(position.z)));
	return ( 0.01 > position.y - getVolume().y - obstacle->getNode()->getPosition().y - obstacle->getVolume().y);
}

void PhysicalObject::addListener(PhysicalObjectListener* listener) {
	mListeners.insert(listener);
}

void PhysicalObject::removeListener(PhysicalObjectListener* listener) {
	mListeners.erase(listener);
}

void PhysicalObject::fireDestruction() const {
	for (std::set<PhysicalObjectListener*>::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it) {
		(*it)->objectDestroyed(this);
	}
}

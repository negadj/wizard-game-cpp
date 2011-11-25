/*
 * PhysicalObject.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "PhysicalObject.h"
#include "ObjectManager.h"

PhysicalObject::PhysicalObject(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, int id, Ogre::String meshName,Ogre::Vector3 volume, Ogre::String description) :
	mObjectManager(objectManager),
	mId(id),
	mName(name),
	mDescription(description),
	mNode(originNode->createChildSceneNode(name)),
	mEntity(0),
	mAcceleration(),
	mSpeed(),
	mSolidity(20),
	mDensity(1),
	mIntegrity(100),
	mVolume(volume),
	mCollisionCorrection(Ogre::Vector3::ZERO)
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

void PhysicalObject::setAcceleration(Ogre::Vector3 mAcceleration)
{
    this->mAcceleration = mAcceleration;
}

void PhysicalObject::setIntegrity(int mIntegrity)
{
    this->mIntegrity = mIntegrity;
}

//void PhysicalObject::setName(std::string mName)
//{
//    this->mName = mName;
//}

float PhysicalObject::getDensity() const
{
    return mDensity;
}

int PhysicalObject::getSolidity() const
{
    return mSolidity;
}

void PhysicalObject::setDensity(float mDensity)
{
    this->mDensity = mDensity;
}

void PhysicalObject::setEntity(Ogre::Entity *mEntity)
{
    this->mEntity = mEntity;
}

Ogre::String PhysicalObject::getDescription() const
{
    return mDescription;
}

ObjectManager* PhysicalObject::getObjectManager() const
{
    return mObjectManager;
}

void PhysicalObject::setDescription(Ogre::String mDescription)
{
    this->mDescription = mDescription;
}

Ogre::Entity *PhysicalObject::getEntity() const
{
    return mEntity;
}

void PhysicalObject::setSolidity(int mSolidity)
{
    this->mSolidity = mSolidity;
}

void PhysicalObject::setSpeed(Ogre::Vector3 mSpeed)
{
    this->mSpeed = mSpeed;
}

void PhysicalObject::addSpeed(Ogre::Vector3 mSpeed)
{
    this->mSpeed += mSpeed;
}

const Ogre::Vector3  PhysicalObject::getVolume() const
{
	return mVolume;
}

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

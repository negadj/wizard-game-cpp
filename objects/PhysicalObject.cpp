/*
 * PhysicalObject.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "PhysicalObject.h"

PhysicalObject::PhysicalObject(const ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, int id, Ogre::String meshName, Ogre::String description) :
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
	mIntegrity(100)
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

const ObjectManager* PhysicalObject::getObjectManager() const
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





/*
 * PhysicalObject.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "PhysicalObject.h"

PhysicalObject::PhysicalObject(Ogre::SceneNode* originNode, int id, Ogre::String name) :
	mId(id),
	mName(name),
	mNode(originNode->createChildSceneNode()),//"Object" + Ogre::StringConverter::toString(PhysicalObject::_instanceNumber)))),
	mAcceleration(),
	mSpeed(),
	mSolidity(20),
	mDensity(1),
	mIntegrity(100)
{
//	setup();
}

PhysicalObject::~PhysicalObject() {
//	PhysicalObject::_instanceNumber -=1;
}


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

//void PhysicalObject::setId(ObjectId_t mId)
//{
//    this->mId = mId;
//}

void PhysicalObject::setIntegrity(int mIntegrity)
{
    this->mIntegrity = mIntegrity;
}

void PhysicalObject::setName(std::string mName)
{
    this->mName = mName;
}

//void PhysicalObject::setNode(Ogre::SceneNode* mNode)
//{
//    this->mNode = mNode;
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

void PhysicalObject::setSolidity(int mSolidity)
{
    this->mSolidity = mSolidity;
}

void PhysicalObject::setSpeed(Ogre::Vector3 mSpeed)
{
    this->mSpeed = mSpeed;
}




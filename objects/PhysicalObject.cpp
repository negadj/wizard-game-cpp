/*
 * PhysicalObject.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "PhysicalObject.h"
#include "ObjectManager.h"

PhysicalObject::PhysicalObject(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, int id, Ogre::String meshName, Ogre::String description) :
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
	mSkeleton(std::vector<std::pair<Ogre::Vector3,Ogre::Vector3> >())
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

void PhysicalObject::setupSkeleton(Ogre::Vector3 cornerMin, Ogre::Vector3 cornerMax, Ogre::Real step)
{
	for(Ogre::Real i = cornerMin.x; i <= cornerMax.x; i+=step)
		for(Ogre::Real j = cornerMin.y; j <= cornerMax.y; j+=step)
			for(Ogre::Real k = cornerMin.z; k <= cornerMax.z; k+=step)
			{
				std::pair<Ogre::Vector3,Ogre::Vector3> tmp(Ogre::Vector3(i,j,k),Ogre::Vector3::ZERO);
				if(i == cornerMax.x)
					tmp.second += Ogre::Vector3::UNIT_X;
				if(i == cornerMin.x)
					tmp.second += Ogre::Vector3::NEGATIVE_UNIT_X;
				if(j == cornerMax.y)
					tmp.second += Ogre::Vector3::UNIT_Y;
				if(j == cornerMin.y)
					tmp.second += Ogre::Vector3::NEGATIVE_UNIT_Y;
				if(k == cornerMax.z)
					tmp.second += Ogre::Vector3::UNIT_Z;
				if(k == cornerMin.z)
					tmp.second += Ogre::Vector3::NEGATIVE_UNIT_Z;
				if(tmp.second != Ogre::Vector3::ZERO)
					mSkeleton.push_back(tmp);
			}
}

const std::vector<std::pair<Ogre::Vector3,Ogre::Vector3> >&  PhysicalObject::getSkeleton()
{
	return mSkeleton;
}




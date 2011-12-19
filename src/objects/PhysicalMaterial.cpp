/*
 * PhysicalMaterial.cpp
 *
 *  Created on: 19 déc. 2011
 *      Author: gecko
 */

#include "PhysicalMaterial.h"

const PhysicalMaterial PhysicalMaterial::Wood("Wizard/Wood", 50, 0.8, true);
const PhysicalMaterial PhysicalMaterial::Grass("Wizard/Grass", 10, 0.5, true);
const PhysicalMaterial PhysicalMaterial::Dirt("Wizard/Dirt", 10, 0.5, true);
const PhysicalMaterial PhysicalMaterial::Iron("Wizard/Iron", 100, 2, true);
const PhysicalMaterial PhysicalMaterial::Flesh("Wizard/Flesh", 20, 2, false);

//PhysicalMaterial::PhysicalMaterial() :
//	mName("Default"),
//	mSolidity(100),
//	mDensity(1),
//	mAffectLook(false)
//{}

PhysicalMaterial::PhysicalMaterial(Ogre::String name, int solidity, float density, bool affectLook) :
	mName(name),
	mSolidity(solidity),
	mDensity(density),
	mAffectLook(affectLook)
{}

PhysicalMaterial::~PhysicalMaterial() {}

float PhysicalMaterial::getDensity() const
{
    return mDensity;
}

Ogre::String PhysicalMaterial::getName() const
{
    return mName;
}

int PhysicalMaterial::getSolidity() const
{
    return mSolidity;
}

void PhysicalMaterial::setDensity(float density)
{
    mDensity = density;
}

void PhysicalMaterial::setName(Ogre::String name)
{
    mName = name;
}

void PhysicalMaterial::setSolidity(int solidity)
{
    mSolidity = solidity;
}

bool PhysicalMaterial::isAffectingLook() const
{
    return mAffectLook;
}

void PhysicalMaterial::setAffectLook(bool affectLook)
{
    mAffectLook = affectLook;
}






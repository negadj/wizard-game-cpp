/*
 * PhysicalMaterial.h
 *
 *  Created on: 19 déc. 2011
 *      Author: gecko
 */

#ifndef PHYSICALMATERIAL_H_
#define PHYSICALMATERIAL_H_

#include "Ogre.h"

class PhysicalMaterial {
public:
	static const PhysicalMaterial Wood;
	static const PhysicalMaterial Dirt;
	static const PhysicalMaterial Grass;
	static const PhysicalMaterial Iron;
	static const PhysicalMaterial Flesh;

//	PhysicalMaterial();
	PhysicalMaterial(Ogre::String name, int solidity, float density, bool affectLook = false);
	virtual ~PhysicalMaterial();
    float getDensity() const;
    Ogre::String getName() const;
    int getSolidity() const;
    void setDensity(float mDensity);
    void setName(Ogre::String mName);
    void setSolidity(int mSolidity);
    bool isAffectingLook() const;
    void setAffectLook(bool affectLook);

private:
    Ogre::String mName;
    int mSolidity;
    float mDensity;
    bool mAffectLook; // Ce matériau change-t-il l'apparence ?
};

#endif /* PHYSICALMATERIAL_H_ */

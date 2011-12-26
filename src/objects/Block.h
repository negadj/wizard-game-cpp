/*
 * Block.h
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "PhysicalObject.h"
#include "../Terrain.h"

/**
 * Représente un bloc de matière à la sauce Minecraft. (1m cube)
 */
class Block: public PhysicalObject {
	friend class ObjectManager;
	friend class Terrain;
protected:
	Block(Ogre::SceneNode* originNode, PhysicalMaterial material);
	virtual ~Block();
	virtual void onIntegrityChange(int oldIntegrity);
};

#endif /* BLOCK_H_ */

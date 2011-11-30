/*
 * Block.h
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "PhysicalObject.h"

/**
 * Représente un bloc de matière à la sauce Minecraft. (1m cube)
 */
class Block: public PhysicalObject {
	friend class ObjectManager;
protected:
	Block(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, int id);

protected:
	virtual ~Block();
};

#endif /* BLOCK_H_ */

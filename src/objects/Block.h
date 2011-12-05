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
	friend bool Terrain::addBlock(Block&); // Pour pouvoir les détacher de la scène pour la static geometry
	friend bool Terrain::addBlocks(const std::vector<Block*>&);
	friend void Terrain::removeBlock(const Triplet&);
protected:
	Block(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name);

protected:
	virtual ~Block();
};

#endif /* BLOCK_H_ */

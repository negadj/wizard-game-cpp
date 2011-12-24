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
	friend void Terrain::attachBlock(Block&, bool); // Pour pouvoir les détacher de la scène pour la static geometry
	friend void Terrain::detachBlock(Block&, bool);
	friend void Terrain::updateTerrain();
protected:
	Block(Ogre::SceneNode* originNode, PhysicalMaterial material);
	virtual ~Block();
	virtual void onIntegrityChange(int oldIntegrity);
	//void attach();
	//void detach();
};

#endif /* BLOCK_H_ */

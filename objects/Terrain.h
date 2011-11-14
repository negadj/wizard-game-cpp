/*
 * Terrain.h
 *
 *  Created on: 10 nov. 2011
 *      Author: gecko
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <Ogre.h>
#include "Triplet.h"
#include "Block.h"

class ObjectManager;

class Terrain {
private:
	const ObjectManager* mObjMgr;
	std::map<Triplet,Ogre::String> mMap;
	Ogre::StaticGeometry* mStaticGeometry;

public:
	Terrain(const ObjectManager* objMgr, Ogre::StaticGeometry* staticGeometry);
	virtual ~Terrain();
	/**
	 * Rajoute le bloc au terrain
	 * (le détache au passage du SceneGraph pour l'incorporer à une StaticGeometry)
	 * Renvoie true si la place était libre, false si la place est occupée (dans ce cas le cube n'est pas ajouté).
	 */
	bool addBlock(Block& b);
	/**
	 * Comme addBlock, mais plus optimisé uand il faut rajouter un grand nombre de blocs.
	 * Renvoie false si l'un des emplacements des blocs était déjà occupé
	 */
	bool addBlocks(const std::vector<Block*> &blocks);
	/**
	 * Renvoie un pointeur sur le bloc à la position choisie,
	 * ou NULL s'il n'y a pas de bloc.
	 */
	Block* getBlock(const Triplet& pos);
	Block* getBlock(const Ogre::Vector3& pos);
	Ogre::String getBlockByName(const Triplet& pos);
	Ogre::String getBlockByName(const Ogre::Vector3& pos);
	bool isFree(const Triplet& pos);
	bool isFree(const Ogre::Vector3& pos);
	void removeBlock(const Triplet& pos);
};

#endif /* TERRAIN_H_ */

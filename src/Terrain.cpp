/*
 * Terrain.cpp
 *
 *  Created on: 10 nov. 2011
 *      Author: gecko
 */

#include "Terrain.h"
#include "ObjectManager.h"

Terrain::Terrain(const ObjectManager* objMgr, Ogre::StaticGeometry* staticGeometry) :
	mObjMgr(objMgr),
	mStaticGeometry(staticGeometry)
{
	mStaticGeometry->setRegionDimensions(Ogre::Vector3::UNIT_SCALE*400);
	mStaticGeometry->setOrigin(Ogre::Vector3(-200,0,-200));
	mStaticGeometry->build();
}

/* Note : ne pas chercher à détruire la static geometry.
 * Elle le sera lorsque la scène sera nettoyée, à la destruction
 * de l'ObjectManager.
 */
Terrain::~Terrain() {}

bool Terrain::addBlock(Block& b) {
	Ogre::Vector3 pos = b.getNode()->getPosition();
	if (isFree(pos)) {
		// On retire d'abord le cube du graphe de scène
		b.getNode()->getParentSceneNode()->removeChild(b.getNode());
		// On rajoute le cube et on reconstruit la StaticGeometry
		mMap[Triplet(pos)] = b.getName();
		mStaticGeometry->destroy();
		mStaticGeometry->addSceneNode(b.getNode());
		mStaticGeometry->build();
		return true;
	}
	else return false;
}

bool Terrain::addBlocks(const std::vector<Block*> &blocs) {
	Block* b;
	Ogre::Vector3 pos;
	bool res = true;
	mStaticGeometry->destroy();
	for (std::vector<Block*>::const_iterator it = blocs.begin(); it != blocs.end(); ++it) {
		b = *it;
		pos = b->getNode()->getPosition();
		if (isFree(pos)) {
			// On retire d'abord le cube du graphe de scène
			b->getNode()->getParentSceneNode()->removeChild(b->getNode());
			// On rajoute le cube et on reconstruit la StaticGeometry
			mMap[Triplet(pos)] = b->getName();
			mStaticGeometry->addSceneNode(b->getNode());
		}
		else
			res = false;
	}
	mStaticGeometry->build();
	return res;
}

bool Terrain::isFree(const Triplet& pos) const {
	return mMap.find(pos) == mMap.end();
}

bool Terrain::isFree(const Ogre::Vector3& pos) const {
	return isFree(Triplet(pos));
}

Block* Terrain::getBlock(const Triplet& pos) const {
	if(isFree(pos))
		return NULL;
	return static_cast<Block*>(mObjMgr->getObject(mMap.at(pos)));
}

Block* Terrain::getBlock(const Ogre::Vector3& pos) const {
	return getBlock(Triplet(pos));
}

Ogre::String Terrain::getBlockByName(const Triplet& pos) const {
	if(isFree(pos))
		return "";
	return mMap.at(pos);
}

Ogre::String Terrain::getBlockByName(const Ogre::Vector3& pos) const {
	return getBlockByName(Triplet(pos));
}

void Terrain::removeBlock(const Triplet& pos) {
	if (isFree(pos))
		return;
	mMap.erase(pos);
	mStaticGeometry->reset();
	/**
	 * On rajoute tout les blocs encore intégré.
	 * Inutile d'utiliser addBlock() sur eux.
	 */
	for (std::map<Triplet,Ogre::String>::iterator it = mMap.begin(); it != mMap.end(); ++it) {
		mStaticGeometry->addSceneNode(mObjMgr->getObject(it->second)->getNode());
	}
	mStaticGeometry->build();
}

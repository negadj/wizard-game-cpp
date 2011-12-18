/*
 * Terrain.cpp
 *
 *  Created on: 10 nov. 2011
 *      Author: gecko
 */

#include "Terrain.h"
#include "ObjectManager.h"
#include "objects/Block.h"

Terrain::Terrain(ObjectManager* objMgr, Ogre::StaticGeometry* staticGeometry) :
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
Terrain::~Terrain() {
#ifdef DEBUG_MODE
LOG("call Terrain destructor");
#endif
}

void Terrain::attachBlock(Block& b, bool update) {
	// On le retire des objets à rendre
	if (b.getNode()->isInSceneGraph()) {
		b.getNode()->getParentSceneNode()->removeChild(b.getNode());

		if (update)
			mStaticGeometry->destroy();
		// On le rajoute à la geometry
		mStaticGeometry->addSceneNode(b.getNode());
		if (update)
			mStaticGeometry->build();
		// On s'enregistre pour surveiller ses changements d'apparance
		b.addListener(this);
	}
}

void Terrain::detachBlock(Block& b, bool update) {
	if (!b.getNode()->isInSceneGraph()) {
		//On raccroche le block
		b.getNode()->getCreator()->getRootSceneNode()->addChild(b.getNode());

		if (update)
			/* On reconstruit la geometry
			 * (impossible de ne retirer qu'un seul bloc, comme pour attachBlock)
			 */
			updateTerrain();

		// Plus la peine d'écouter les changement d'apparence
		b.removeListener(this);
	}
}

bool Terrain::addBlock(Block& b) {
	Ogre::Vector3 pos = b.getPosition();
	if (isFree(pos)) {
		mMap[Triplet(pos)] = b.getName();
		attachBlock(b);
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
		pos = b->getPosition();
		if (isFree(pos)) {
			// On reconstruit la StaticGeometry
			mMap[Triplet(pos)] = b->getName();
			// On retire d'abord le cube du graphe de scène
			attachBlock(*b, false);
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

Block* Terrain::getBlock(const Triplet& pos) {
	if(isFree(pos))
		return NULL;
	return static_cast<Block*>(mObjMgr->getObject(mMap[pos]));
}

Block* Terrain::getBlock(const Ogre::Vector3& pos) {
	return getBlock(Triplet(pos));
}

Ogre::String Terrain::getBlockName(const Triplet& pos) {
	if(isFree(pos))
		return "";
	return mMap[pos];
}

Ogre::String Terrain::getBlockName(const Ogre::Vector3& pos) {
	return getBlockName(Triplet(pos));
}

void Terrain::updateTerrain() {
	mStaticGeometry->reset();
	Block* toAdd;
	for (std::map<Triplet,Ogre::String>::iterator it = mMap.begin(); it != mMap.end(); ++it) {
		toAdd = static_cast<Block*>(mObjMgr->getObject(it->second));
		if (!toAdd->getNode()->isInSceneGraph())
			mStaticGeometry->addSceneNode(toAdd->getNode());
	}
	mStaticGeometry->build();
}

void Terrain::removeBlock(const Triplet& pos) {
#ifdef DEBUG_MODE
LOG("enter Terrain::removeBlock");
#endif
	if (isFree(pos))
		return;

	detachBlock(*getBlock(pos));
	mMap.erase(pos);

#ifdef DEBUG_MODE
LOG("exit Terrain::removeBlock");
#endif
}

void Terrain::objectApparenceChanged(const PhysicalObject* object) {
	updateTerrain();
	//detachBlock(static_cast<Block*>(object));
}

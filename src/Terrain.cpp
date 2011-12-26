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
	mMapManager(10),
	mStaticGeometry(staticGeometry),
	mChunks(std::set<Triplet>())
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
			refreshTerrain();

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

void Terrain::refreshTerrain() {
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

/*
 * Note : la geometry est reconstruite dans manage().
 */
void Terrain::loadChunk(const Triplet& chunk) {
#ifdef DEBUG_MODE
LOG("load chunk " + Ogre::StringConverter::toString(Ogre::Vector3(chunk)));
#endif
	// On créé les blocs associés
	std::vector<std::pair<Triplet,PhysicalMaterial> > blocks_description = mMapManager.loadChunk(chunk);
	std::vector<Block*> blocks;
	for (std::vector<std::pair<Triplet,PhysicalMaterial> >::iterator it = blocks_description.begin(); it != blocks_description.end(); ++it) {
		blocks.push_back(mObjMgr->createBlock(it->first,it->second, false));
	}

	// Les blocs sont rajoutés au terrain
	Block* b;
	Ogre::Vector3 pos;
	for (std::vector<Block*>::const_iterator it = blocks.begin(); it != blocks.end(); ++it) {
		b = *it;
		pos = b->getPosition();
		mMap[Triplet(pos)] = b->getName();
		attachBlock(*b, false);
	}

	// On indique que le chunk est chargé.
	mChunks.insert(chunk);
}

/*
 * Pour supprimer proprement les blocs de l'ObjectManager,
 * on fait une requete de suppression pour chaque bloc.
 * Pour ne pas perdre trop en performance, on les supprime d'abord
 * du terrain dès cette méthode.
 */
void Terrain::unloadChunk(const Triplet& chunk) {
#ifdef DEBUG_MODE
LOG("unload chunk " + Ogre::StringConverter::toString(Ogre::Vector3(chunk)));
#endif
	// Les blocs sont retirés
	Triplet pos;
	int size = mMapManager.getChunkSize();
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			for (int k=0; k<size; k++) {
				pos = Triplet(Ogre::Vector3(chunk) * size + Ogre::Vector3(i,j,k));
				if (!isFree(pos)) { // On retire chaque bloc du terrain
					mObjMgr->requestDestruction(getBlock(pos)->getName());
					detachBlock(*getBlock(pos), false);
					mMap.erase(pos);
				}
			}
		}
	}
	// On note que le chunk a bien été déchargé
	mChunks.erase(chunk);
}

void Terrain::manage(const std::set<PhysicalObject*>& travelers) {
	Triplet chunkLocation;
	std::set<Triplet> chunks = std::set<Triplet>();
	bool modified = false;
	for (std::set<PhysicalObject*>::iterator it = travelers.begin(); it != travelers.end(); ++it) {
		// On rajoute les chunks à conserver/rajouter dans la liste
		chunkLocation = Ogre::Vector3((*it)->getPosition()) / mMapManager.getChunkSize();
		for (int i=-1; i<2; i++)
			for (int j=-1; j<2; j++) {
				//TODO k?
				chunks.insert(Triplet(chunkLocation.x + i, chunkLocation.y, chunkLocation.z +j));

			}
	}
	// On décharge les chunks inutiles
	for (std::set<Triplet>::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
		if (chunks.find(*it) == chunks.end()) {
			unloadChunk(*it);
			modified = true;
		}

	// Les nouveaux chunks sont chargés
	for (std::set<Triplet>::iterator it = chunks.begin(); it != chunks.end(); ++it)
		if (mChunks.find(*it) == mChunks.end()) {
					loadChunk(*it);
					modified = true;
		}

	if (modified) // On reconstruit le terrain si nécessaire
		refreshTerrain();
}

void Terrain::objectApparenceChanged(const PhysicalObject* object) {
	refreshTerrain();
}

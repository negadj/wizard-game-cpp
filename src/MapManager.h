/*
 * MapManager.h
 *
 *  Created on: 21 nov. 2011
 *      Author: corum
 */

#ifndef MAPMANAGER_H_
#define MAPMANAGER_H_

#include <Ogre.h>
#include <fstream>
#include "Triplet.h"
#include "objects/PhysicalMaterial.h"

/*
 * Gère l'import d'une map
 */

class MapManager {

private:
	int mChunkSize;

	std::vector<std::pair<Triplet,PhysicalMaterial> > loadChunkFromFile(Triplet chunkPosition, std::string filename);
	std::vector<std::pair<Triplet,PhysicalMaterial> > generateChunk(Triplet chunkPosition);

public:
	MapManager(int chunkSize);
	virtual ~MapManager();
	std::vector<std::pair<Triplet,PhysicalMaterial> > loadChunk(Triplet chunkPosition);
};

#endif /* MAPMANAGER_H_ */

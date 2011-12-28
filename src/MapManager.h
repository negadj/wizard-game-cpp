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

class Block;

/*
 * Gère l'import d'une map
 */
class MapManager {

private:
	int mChunkSize;

	std::string getFilename(const Triplet& chunk);
	std::vector<std::pair<Triplet,PhysicalMaterial> > loadChunkFromFile(Triplet chunkPosition, std::string filename);
	std::vector<std::pair<Triplet,PhysicalMaterial> > generateChunk(Triplet chunkPosition);

public:
	MapManager(int chunkSize);
	virtual ~MapManager();
	int getChunkSize();
	std::vector<std::pair<Triplet,PhysicalMaterial> > loadChunk(Triplet chunkPosition);
	void saveChunk(std::vector<Block*> blocks);
};

#endif /* MAPMANAGER_H_ */

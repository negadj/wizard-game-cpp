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

class MapManager {

private:
	int mChunkSize;
public:
	MapManager(int chunkSize);
	virtual ~MapManager();
	std::vector<Ogre::Vector3>  loadChunk(Ogre::Vector3 chunkPosition);
};

#endif /* MAPMANAGER_H_ */

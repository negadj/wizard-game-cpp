/*
 * MapManager.cpp
 *
 *  Created on: 21 nov. 2011
 *      Author: corum
 */

#include "MapManager.h"

#ifdef DEBUG_MODE
#define LOG(x) Ogre::LogManager::getSingleton().logMessage(x)
#endif

MapManager::MapManager(int chunkSize):
	mChunkSize(chunkSize)
{
}

MapManager::~MapManager()
{
#ifdef DEBUG_MODE
LOG("call MapManager destructor");
#endif
}

std::vector<std::pair<Triplet,PhysicalMaterial> > MapManager::loadChunk(Triplet chunkPosition)
{
	std::string filename = "map/x" + Ogre::StringConverter::toString(int(chunkPosition.x)) +
						   "y" + Ogre::StringConverter::toString(int(chunkPosition.y)) +
						   "z" + Ogre::StringConverter::toString(int(chunkPosition.z));
	std::ifstream chunk(filename.c_str());
	if (chunk) { // Le fichier existe
		chunk.close();
		return loadChunkFromFile(chunkPosition, filename);
	}
	return generateChunk(chunkPosition);
}

std::vector<std::pair<Triplet,PhysicalMaterial> > MapManager::generateChunk(Triplet chunkPosition)
{
	std::vector<std::pair<Triplet,PhysicalMaterial> > result = std::vector<std::pair<Triplet,PhysicalMaterial> > ();

	int altitude = 0;
//	PhysicalMaterial material;
	for (int i=0; i<mChunkSize; i++) {
		for (int j=0; j<mChunkSize; j++) {
			if (rand()%2) {
				// On change l'altitude
				if (rand()%2)
					altitude++;
				else
					altitude--;
			}
			for (int k=-10; k<altitude; k++) {
				if (k >= 4) {
					if (rand()%4)
						result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i, chunkPosition.z * mChunkSize + k, chunkPosition.z * mChunkSize +j), PhysicalMaterial::Dirt));
					else
						result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i, chunkPosition.z * mChunkSize + k, chunkPosition.z * mChunkSize +j), PhysicalMaterial::Grass));
				}
				else if (k<0){
					result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i, chunkPosition.z * mChunkSize + k, chunkPosition.z * mChunkSize +j), PhysicalMaterial::Dirt));
				}
				else {
					result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i, chunkPosition.z * mChunkSize + k, chunkPosition.z * mChunkSize +j), PhysicalMaterial::Grass));
				}
			}
		}
	}
	return result;
}

std::vector<std::pair<Triplet,PhysicalMaterial> > MapManager::loadChunkFromFile(Triplet chunkPosition, std::string filename) {
	std::vector<std::pair<Triplet,PhysicalMaterial> > result = std::vector<std::pair<Triplet,PhysicalMaterial> > ();

	std::ifstream chunk(filename.c_str());
	int i = 0;
	int k = 0;
	char c = '\0';
	while(chunk.get(c))
	{
		if(c == '\n')
		{
			i = 0;
			++k;
		}
		else if( c == ' ')
		{
			result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize , chunkPosition.z * mChunkSize +k), PhysicalMaterial::Grass));
			i++;
		}
		else if( c == 'd')
		{
			result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize , chunkPosition.z * mChunkSize +k), PhysicalMaterial::Grass));
			result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize + 1 , chunkPosition.z * mChunkSize +k), PhysicalMaterial::Dirt));
			result.push_back(std::pair<Triplet,PhysicalMaterial>(Triplet(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize + 2, chunkPosition.z * mChunkSize +k), PhysicalMaterial::Dirt));
			i++;
		}
	}
	chunk.close();
	return result;
}

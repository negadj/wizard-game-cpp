/*
 * MapManager.cpp
 *
 *  Created on: 21 nov. 2011
 *      Author: corum
 */

#include "MapManager.h"
#include "objects/Block.h"

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

std::string MapManager::getFilename(const Triplet& chunk) {
	return "map/x" + Ogre::StringConverter::toString(int(chunk.x)) +
							   "y" + Ogre::StringConverter::toString(int(chunk.y)) +
							   "z" + Ogre::StringConverter::toString(int(chunk.z));
}

int MapManager::getChunkSize() {
	return mChunkSize;
}

std::vector<std::pair<Triplet,PhysicalMaterial> > MapManager::loadChunk(Triplet chunkPosition)
{
	std::string filename = getFilename(chunkPosition);
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
	Triplet coord;
	for (int i=0; i<mChunkSize; i++) {
		for (int j=0; j<mChunkSize; j++) {
			for (int k=0; k<mChunkSize; k++) {
				coord = Triplet(Ogre::Vector3(chunkPosition)*mChunkSize + Ogre::Vector3(i,j,k));
				if (coord.y < 1) {
					if (coord.y >= 0)
						result.push_back(std::pair<Triplet,PhysicalMaterial>(coord, PhysicalMaterial::Grass));
					else
						result.push_back(std::pair<Triplet,PhysicalMaterial>(coord, PhysicalMaterial::Dirt));
				}
			}
		}
	}
	return result;
}

std::vector<std::pair<Triplet,PhysicalMaterial> > MapManager::loadChunkFromFile(Triplet chunkPosition, std::string filename) {
	std::vector<std::pair<Triplet,PhysicalMaterial> > result = std::vector<std::pair<Triplet,PhysicalMaterial> > ();

	std::ifstream chunk(filename.c_str());
	Ogre::String line = "";
	Triplet coord;
	char type;

	// On parse le fichier
	while (std::getline(chunk, line)) {
		type = line.at(0);
		coord = Triplet(Ogre::StringConverter::parseVector3(line.substr(1)));

		switch(type) {
		case 'd':
			result.push_back(std::pair<Triplet,PhysicalMaterial>(coord, PhysicalMaterial::Dirt));
			break;
		case 'g':
			result.push_back(std::pair<Triplet,PhysicalMaterial>(coord, PhysicalMaterial::Grass));
			break;
		default:
			//TODO: gerer les erreurs
			break;
		}
	}

	chunk.close();
	return result;
}

void MapManager::saveChunk(std::vector<Block*> blocks) {
	if (blocks.size() == 0)
		return;

	Triplet chunkArea = blocks[0]->getPosition() / getChunkSize();
	std::ofstream chunk(getFilename(chunkArea).c_str());
	if (chunk) {
		std::string material;
		for (std::vector<Block*>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
			material = (*it)->getMaterialName();
			if (material.find("Grass") != std::string::npos) {
				material = 'g';
			}
			else
				material = 'd';
			chunk << material << Ogre::StringConverter::toString((*it)->getPosition()) << std::endl;
		}
		chunk.close();
	}
	//TODO: gestion des erreurs ?
}

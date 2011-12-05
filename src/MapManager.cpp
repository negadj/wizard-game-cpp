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

std::vector<Ogre::Vector3> MapManager::loadChunk(Ogre::Vector3 chunkPosition)
{
	std::vector<Ogre::Vector3>  result = std::vector<Ogre::Vector3> ();
/*	std::string filename = "map/x" + Ogre::StringConverter::toString(int(chunkPosition.x)) +
							"y" + Ogre::StringConverter::toString(int(chunkPosition.y)) +
							"z" + Ogre::StringConverter::toString(int(chunkPosition.z));
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
			result.push_back(Ogre::Vector3(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize , chunkPosition.z * mChunkSize +k));
			i++;
		}
		else if( c == 'x')
		{
			result.push_back(Ogre::Vector3(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize , chunkPosition.z * mChunkSize +k));
			result.push_back(Ogre::Vector3(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize + 1 , chunkPosition.z * mChunkSize +k));
			result.push_back(Ogre::Vector3(chunkPosition.x * mChunkSize + i,chunkPosition.y * mChunkSize + 2, chunkPosition.z * mChunkSize +k));
			i++;
		}
	}
	chunk.close();*/
	for (int i=-50; i<50; i++)
		for (int j = -1; j < 1; j++)
			for (int k=-50; k<50; k++)
				result.push_back(Ogre::Vector3(i,j,k));
	return result;
}

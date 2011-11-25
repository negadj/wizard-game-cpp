/*
 * MapManager.cpp
 *
 *  Created on: 21 nov. 2011
 *      Author: corum
 */

#include "MapManager.h"

MapManager::MapManager(int chunkSize):
	mChunkSize(chunkSize)
{
}

MapManager::~MapManager()
{
}

std::vector<Ogre::Vector3> MapManager::loadChunk(Ogre::Vector3 chunkPosition)
{
	std::vector<Ogre::Vector3>  result = std::vector<Ogre::Vector3> ();
	std::ifstream chunk("config/terrain/x0y0z0");
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
	chunk.close();
	return result;
}

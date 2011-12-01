/*
 * Block.cpp
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#include "Block.h"

Block::Block(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name) :
	PhysicalObject(objectManager, originNode, name, TYPE_BLOCK, "cube.mesh",Ogre::Vector3(0.5,0.5,0.5), "Bloc")
{
	getEntity()->setMaterialName("Wizard/Dirt");
	getNode()->scale(0.01*Ogre::Vector3::UNIT_SCALE);
	getNode()->setInitialState();
}

Block::~Block() {}

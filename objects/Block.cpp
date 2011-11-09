/*
 * Block.cpp
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#include "Block.h"

Block::Block(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, int id) :
	PhysicalObject(objectManager, originNode, name, id, "cube.mesh",Ogre::Vector3(1,1,1), "Bloc")
{
	getEntity()->setMaterialName("Wizard/Dirt");
	getNode()->scale(0.01*Ogre::Vector3::UNIT_SCALE);
	getNode()->setInitialState();
}

Block::~Block() {}

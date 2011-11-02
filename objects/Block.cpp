/*
 * Block.cpp
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#include "Block.h"

Block::Block(Ogre::SceneNode* node, Ogre::String name, int id) :
	PhysicalObject(node, name, id, "cube.mesh", "Bloc")
{
	getEntity()->setMaterialName("Wizard/Dirt");
	getNode()->scale(0.01*Ogre::Vector3::UNIT_SCALE);
	getNode()->setInitialState();
}

Block::~Block() {}

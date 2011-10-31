/*
 * Cube.cpp
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#include "Cube.h"

Cube::Cube(Ogre::SceneNode* node, Ogre::String name, int id) :
	PhysicalObject(node, name, id, "cube.mesh", "Cube")
{
	getEntity()->setMaterialName("Wizard/Dirt");
	getNode()->scale(0.01*Ogre::Vector3::UNIT_SCALE);
	getNode()->setInitialState();
}

Cube::~Cube() {}

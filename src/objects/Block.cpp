/*
 * Block.cpp
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#include "Block.h"
#include "../ObjectManager.h"

Block::Block(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, PhysicalMaterial material) :
	PhysicalObject(objectManager, originNode, name, "cube.mesh",Ogre::Vector3(0.5,0.5,0.5), material, "Bloc")
{
	registerType(TYPE_BLOCK);
	getNode()->scale(0.01*Ogre::Vector3::UNIT_SCALE);
	getNode()->setInitialState();
}

Block::~Block() {}

void Block::onIntegrityChange(int oldIntegrity) {
	if (getIntegrity() > 90) {
		getEntity()->setMaterialName(getMaterialName());
		fireApparenceChanged();
	}
	else if (getIntegrity() > 0 && getIntegrity() <= 90) {
		getEntity()->setMaterialName(getMaterialName()+"Broken");
		fireApparenceChanged();
	}
	else
		die();
}


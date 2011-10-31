/*
 * Cube.h
 *
 *  Created on: 31 oct. 2011
 *      Author: gecko
 */

#ifndef CUBE_H_
#define CUBE_H_

#include "PhysicalObject.h"

class Cube: public PhysicalObject {
	friend class ObjectManager;
protected:
	Cube(Ogre::SceneNode* node, Ogre::String name, int id);

protected:
	virtual ~Cube();
};

#endif /* CUBE_H_ */

/*
 * ExplorerIA.h
 *
 *  Created on: 25 nov. 2011
 *      Author: corum
 */

#ifndef EXPLORERIA_H_
#define EXPLORERIA_H_

#include "IA.h"

class ExplorerIA: public IA {
public:
	ExplorerIA(Monster* monster);
	virtual ~ExplorerIA();
	Ogre::Vector3 findDirection();
};
#endif /* EXPLORERIA_H_ */

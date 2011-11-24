/*
 * PatrollIA.h
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#ifndef PATROLLIA_H_
#define PATROLLIA_H_

#include "IA.h"

class PatrollIA: public IA {
public:
	PatrollIA(Monster* monster);
	virtual ~PatrollIA();
	Ogre::Vector3 findDirection();
};

#endif /* PATROLLIA_H_ */

/*
 * SimpleIA.h
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#ifndef SIMPLEIA_H_
#define SIMPLEIA_H_

#include "IA.h"


class RandomIA: public IA {
public:
	RandomIA(Monster* monster);
	virtual ~RandomIA();
	Ogre::Vector3 findDirection();
};

#endif /* SIMPLEIA_H_ */

/*
 * IA.h
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#ifndef IA_H_
#define IA_H_

#include <Ogre.h>

class IA;

#include "Monster.h"

class IA {
protected:
	Monster* mMonster;
public:
	IA(Monster* monster);
	~IA();
	virtual Ogre::Vector3 findDirection() =0;
	static IA* getIA(Monster* monster);
};

#endif /* IA_H_ */

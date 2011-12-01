/*
 * GuardianIA.h
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#ifndef GUARDIANIA_H_
#define GUARDIANIA_H_

#include "IA.h"

/*
 * IA ne se déplaçant pas. Elle reste sur place
 */
class GuardianIA: public IA {
public:
	GuardianIA(Monster* monster);
	virtual ~GuardianIA();
	Ogre::Vector3 findDirection();
};

#endif /* GUARDIANIA_H_ */

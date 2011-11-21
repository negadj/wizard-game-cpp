/*
 * Triplet.h
 *
 *  Created on: 9 nov. 2011
 *      Author: corum
 */

#ifndef TRIPLET_H_
#define TRIPLET_H_

#include "Ogre.h"

class Triplet {
public:
	double x;
	double y;
	double z;
	Triplet();
	Triplet(double x,double y,double z);
	Triplet(Ogre::Vector3 vector);
	virtual ~Triplet();
};

bool operator==(Triplet const& a, Triplet const& b);

bool operator<(Triplet const& a, Triplet const& b);

#endif /* TRIPLET_H_ */

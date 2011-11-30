/*
 * Triplet.h
 *
 *  Created on: 9 nov. 2011
 *      Author: corum
 */

#ifndef TRIPLET_H_
#define TRIPLET_H_

#include "Ogre.h"

/*
 * Surcharge des vecteurs permettant d'avoir un ordre global.
 */
class Triplet {
public:
	static const Triplet ZERO;
	double x;
	double y;
	double z;
	Triplet();
	Triplet(double x,double y,double z);
	Triplet(const Ogre::Vector3& vector);
	virtual ~Triplet();
	operator Ogre::Vector3() const;
};

bool operator==(Triplet const& a, Triplet const& b);

bool operator<(Triplet const& a, Triplet const& b);

Ogre::Vector3 round(const Ogre::Vector3 vector);

Ogre::Vector3 floor(const Ogre::Vector3 vector);

#endif /* TRIPLET_H_ */

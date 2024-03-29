/*
 * ExplorerIA.h
 *
 *  Created on: 25 nov. 2011
 *      Author: corum
 */

#ifndef EXPLORERIA_H_
#define EXPLORERIA_H_

#include "IA.h"

/*
 *  IA allant tout droit jusqu'a ce qu'il rencontre un obstacle. ALors, elle prend une direction aléatoirement sur les trois restantes
 */

class ExplorerIA: public IA {
public:
	ExplorerIA(AnimatedObject* character);
	virtual ~ExplorerIA();
	virtual void takeADecision();
};
#endif /* EXPLORERIA_H_ */

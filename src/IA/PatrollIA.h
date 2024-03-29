/*
 * PatrollIA.h
 *
 *  Created on: 24 nov. 2011
 *      Author: corum
 */

#ifndef PATROLLIA_H_
#define PATROLLIA_H_

#include "IA.h"

/*
 * IA qui "patrouille", c'est a dire qui va out droit jusqu'a rentrer dans un obstacle puis fait demi tour
 */
class PatrollIA: public IA {
public:
	PatrollIA(AnimatedObject* character);
	virtual ~PatrollIA();
	virtual void takeADecision();
};

#endif /* PATROLLIA_H_ */

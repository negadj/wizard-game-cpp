/*
 * IA.h
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#ifndef IA_H_
#define IA_H_

//forward declaration
class IA;

#include <Ogre.h>
#include "../objects/AnimatedObject.h"

/*
 * IA générique, sans intelligence implémentée
 */
class IA {
protected:
	AnimatedObject* mCharacter;
public:
	/*
	 * Permet de récupérer aléatoirement un type d'IA
	 */
	static IA* getRandomIA(AnimatedObject* character);

	IA(AnimatedObject* character);
	virtual ~IA();
	/*
	 * Choisir une action à effectuer
	 */
	virtual void takeADecision() =0;

	AnimatedObject* getCharacter();
};

#endif /* IA_H_ */

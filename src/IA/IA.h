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
#include "../objects/Monster.h"

/*
 * IA générique, sans intelligence implémentée
 */
class IA {
protected:
	Monster* mMonster;
public:
	IA(Monster* monster);
	virtual ~IA();
	/*
	 * Méthode virtuelle permettant de choisir le chemain
	 */
	virtual Ogre::Vector3 findDirection() =0;
	/*
	 * Permet de récupérer aléatoirement un type d'IA
	 */
	static IA* getIA(Monster* monster);
};

#endif /* IA_H_ */

/*
 * Monster.h
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include <Ogre.h>
#include "PhysicalObject.h"

class Monster;

#include "../IA/IA.h"

/*
 * Represente un monstre controle par une intelligence artificielle
 */
class Monster: public PhysicalObject
{
public:
	Monster(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name);
	virtual ~Monster();
	virtual void update(Ogre::Real deltaTime);

private:
	Ogre::SceneNode* mBodyNode;
	Ogre::Entity* mSword1;
	Ogre::Entity* mSword2;
	Ogre::Vector3 mDirection; // direction du joueur donnée par les touches du clavier
	Ogre::Real mPropulsion;
	Ogre::Real mVerticalVelocity; // for jumping
	IA* mIA;
	void setupBody(Ogre::SceneNode* originNode);
};

#endif /* MONSTER_H_ */

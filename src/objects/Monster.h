/*
 * Monster.h
 *
 *  Created on: 22 nov. 2011
 *      Author: corum
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include <Ogre.h>
#include "AnimatedObject.h"

class Monster;

#include "../IA/IA.h"

/*
 * Represente un monstre controle par une intelligence artificielle
 */
class Monster: public AnimatedObject
{
public:
	Monster(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name);
	virtual ~Monster();
	virtual void doPreCollisionUpdate(Ogre::Real deltaTime);
	virtual void doPostCollisionUpdate(Ogre::Real deltaTime);

private:
	Ogre::SceneNode* mBodyNode;
	IA* mIA;
	void setupBody(Ogre::SceneNode* originNode);
};

#endif /* MONSTER_H_ */

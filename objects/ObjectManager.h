/*
 * ObjectManager.h
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "Ogre.h"
#include "../CollisionTools.h"
#include "PhysicalObject.h"
#include "Player.h"
#include "Block.h"
#include "Triplet.h"
#include "Clock.h"

/**
 * Classe qui s'occupe de la gestion des objets matériels du jeu.
 * C'est via cette classe que tous les objets doivent être créés.
 */
class ObjectManager {
private:
	static unsigned long _countObject;
	Ogre::SceneManager* mSceneMgr;
	MOC::CollisionTools mCollisionTools;
	std::map<std::string,PhysicalObject*> mObjects;
	std::vector<PhysicalObject*> mActiveObjects;
	std::map<Triplet,Ogre::String> mTerrain;
	Clock mPhysicalClock;

	void moveWithCollisions(PhysicalObject* &obj, const Ogre::Real deltaTime);
public:
	ObjectManager(Ogre::SceneManager* scnMgr);
	~ObjectManager();
	/*
	 * Met à jour les objets actifs de la scène. Inclus la gestion "moteur physique" (collisions, ...)
	 */
	void updateObjects(Ogre::Real deltaTime);
	/*
	 * Ajoute un joueur
	 */
	Player* createPlayer(Ogre::Camera* camera);
	/*
	 * Ajoute un Block
	 */
	Block* createBlock(const Ogre::Vector3 position = Ogre::Vector3::ZERO);
	/*
	 * Renvoie si un objet est atteint, et si oui lequel
	 */
	bool objectReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, PhysicalObject* &target);
	/*
	 * Charge la scène où le joueur évolue.
	 * TODO: clarifier son rôle vis-à-vis de OgreApplication::createScene()
	 */
	void loadScene();
	/**
	 * détruit tous les objets de la scène
	 */
	void clear();

	void handleCollision(const PhysicalObject* obj, Vector3 &deplacement);

	Vector3 getGravity(PhysicalObject* obj) const;

	double getStrench(PhysicalObject* obj) const;

	bool isOnGround(PhysicalObject* obj) const;
};

#endif /* OBJECTMANAGER_H_ */

/*
 * PhysicManager.h
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
#include "Cube.h"

/**
 * Classe qui s'occupe de la gestion des objets matériels du jeu.
 * C'est via cette classe que tous les objets doivent être créés.
 */
class ObjectManager {
private:
	static unsigned long _countObject;
	Ogre::SceneManager* mSceneMgr;
//	Ogre::RaySceneQuery* mRSQ;
	MOC::CollisionTools mCollisionTools;
	std::vector<PhysicalObject*> mObjects;
	std::vector<PhysicalObject*> mActiveObjects;
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
	 * Ajoute un cube
	 */
	Cube* createCube(Ogre::Vector3 position = Ogre::Vector3::ZERO);
	void loadScene();

};

#endif /* OBJECTMANAGER_H_ */

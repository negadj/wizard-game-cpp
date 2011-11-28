/*
 * ObjectManager.h
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "Ogre.h"
#include "CollisionManager.h"
#include "objects/PhysicalObject.h"
#include "objects/Player.h"
#include "objects/Block.h"
#include "Terrain.h"
#include "Clock.h"
#include "MapManager.h"
#include "objects/Monster.h"

#ifdef DEBUG_MODE
#define LOG(x) Ogre::LogManager::getSingleton().logMessage(x)
#endif

/**
 * Classe qui s'occupe de la gestion des objets matériels du jeu.
 * C'est via cette classe que tous les objets doivent être créés.
 */
class ObjectManager {
private:
	static unsigned long _countObject; // Sert à attribuer un numéro unique à chaque objet.
	Ogre::SceneManager* mSceneMgr;
	CollisionManager mCollisionMgr;
	std::map<std::string,PhysicalObject*> mObjects; // Tous les objets créés.
	/*
	 * Sous-ensemble d'objets dits "actifs" car susceptibles de se déplacer
	 * et d'interagir avec l'environnement.
	 * De manière générale, ce sont tous les êtres vivants, mais il n'est pas exclu
	 * qu'on y mette un jour autre chose, ou que des êtres vivants soient temporairement retiré de la liste.
	 * Permet d'optimiser les calculs en se concentrant sur les objets qui évoluent.
	 */
	std::vector<PhysicalObject*> mActiveObjects;
	/*
	 *
	 */
	Terrain mTerrain;
	Clock mPhysicalClock;
	MapManager mMapManager;

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
	 * Ajoute un monstre
	 */
	Monster* createMonster(const Ogre::Vector3 position = Ogre::Vector3::ZERO);
	/*
	 * Renvoie si un objet est à portée, et si oui lequel.
	 * Note : ne renvoie pas pour l'instant si le terrain est atteint. Voir blockReached pour ça.
	 *
	 * from : position de départ
	 * normal : direction de la visée
	 * reachRadius : portée maximale
	 * target : pointeur pour stocker l'objet atteint le cas échéant
	 */
	bool objectReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, PhysicalObject* &target);
	/*
	 * Indique si un bloc du terrain est à portée, et si oui lequel.
	 *
	 * from : position de départ
	 * normal : direction de la visée
	 * reachRadius : portée maximale
	 * target : pointeur pour stocker le bloc atteint le cas échéant
	 */
	bool blockReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, Block* &target);
		/*
	 * Charge la scène où le joueur évolue.
	 * TODO: clarifier son rôle vis-à-vis de OgreApplication::createScene()
	 */
	void loadScene();
	/**
	 * détruit tous les objets de la scène
	 */
	void clear();

	Vector3 getGravity(PhysicalObject* obj) const;

	double getStrench(PhysicalObject* obj) const;

	Terrain& getTerrain();
	
	PhysicalObject* getObject(const Ogre::String& name) const;
};

#endif /* OBJECTMANAGER_H_ */

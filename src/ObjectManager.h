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
#include "objects/PhysicalObjectListener.h"
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
class ObjectManager : public PhysicalObjectListener {
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
	std::set<PhysicalObject*> mActiveObjects;

	/*
	 * Voir requestRemoval() pour le rôle de cette liste
	 */
	std::queue<Ogre::String> mObjectsToDelete;
	Terrain mTerrain;
	Clock mPhysicalClock;
	Clock mMonsterClock;
	MapManager mMapManager;

	/*
	 * Renvoie un nom unique pour définir un objet
	 */
	Ogre::String getUniqueName();

	/*
	 * Enregistre le nom d'un objet à retirer de la scène et détruire.
	 * On ne le détruit pas immédiatement car il peut ne pas avoir terminé
	 * tout ses traitements au moment où l'on reçoit l'information
	 * (en particulier prévenir tout ses listeners).
	 */
	void requestRemoval(Ogre::String name);

	/*
	 * Retire un objet de la scène.
	 */
	void removeObject(PhysicalObject* object);

	/*
	 * Méthode à utiliser lors de la création d'un PhysicalObject.
	 * Automatise l'enregistrement dans les différentes structures.
	 */
	void registerObject(PhysicalObject* object, bool active = false);

public:
	ObjectManager(Ogre::SceneManager* scnMgr);
	virtual ~ObjectManager();
	/*
	 * Met à jour les objets actifs de la scène. Inclus la gestion "moteur physique" (collisions, ...)
	 */
	void updateObjects(Ogre::Real deltaTime);
	/*
	 * Ajoute un joueur
	 */
	Player* createPlayer(Ogre::Camera* camera);
	/*
	 * Ajoute un Block.
	 * Par défaut il est considéré comme appartenant au terrain,
	 * mais on peut spécifier le contraire avec le second paramètre.
	 */
	Block* createBlock(const Triplet& position = Triplet::ZERO, bool addToTerrain = true);

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
	bool blockReached(const Ogre::Vector3 &from, const Ogre::Vector3 &normal, Ogre::Real reachRadius, Block* &target, Ogre::Vector3* faceVector=NULL);
    /*
	 * Charge la scène où le joueur évolue.
	 */
	void loadScene();
	/**
	 * détruit tous les objets de la scène
	 */
	void clear();
	/*
	 * Indique si un bloc est libre
	 */
	bool isBlockFree(const Triplet& pos) const;
	/*
	 * Renvoie le bloc situé à la position indiquée, ou NULL s'il n'y en a pas.
	 */
	Block* getBlock(const Triplet& pos);

	Vector3 getGravity(PhysicalObject* obj) const;

	double getStrench(PhysicalObject* obj) const;

	PhysicalObject* getObject(const Ogre::String& name);

	virtual void objectDestroyed(const PhysicalObject* object);
};

#endif /* OBJECTMANAGER_H_ */

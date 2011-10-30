/*
 * PhysicManager.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "ObjectManager.h"

/**
 * Classe qui s'occupe de la gestion des objets matériels du jeu.
 * C'est via cette classe que tous les objets doivent être créés.
 */
ObjectManager::ObjectManager(Ogre::SceneManager* scnMgr) :
	mSceneMgr(scnMgr),
//	mRSQ(scnMgr->creater),
	mCollisionTools(scnMgr)
{}

ObjectManager::~ObjectManager() {}

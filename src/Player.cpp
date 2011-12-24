/*
 * Player.cpp
 *
 *  Created on: 24 oct. 2011
 *      Author: gecko
 */

#include "Player.h"
#include "ObjectManager.h"
#include "OgreApplication.h"

Player::Player(OgreApplication* app, AnimatedObject* character, Ogre::Camera* cam) :
	IA(character),
	mApp(app),
	mCharacter(character),
	mOldIA(0),
	mCamera(cam),
	mCameraRootNode(0),
	mCameraGoal(0),
	mCameraFPNode(0),
	mCameraTPNode(0),
	mDirection(Ogre::Vector3::ZERO)
{
	setupCamera();
	setupCharacter(character);
}

Player::~Player() { //Note : les noeuds sont détachés du scene graph, il faut que la scène soit nettoyée !
	cleanCharacter();
}

void Player::injectKeyDown(const OIS::KeyEvent& evt) {
	if (evt.key == mApp->getConfig().getIntValue(mApp->MoveForward))
		mDirection.z = -1;
	else if (evt.key == mApp->getConfig().getIntValue(mApp->MoveBackward))
		mDirection.z = 1;
	else if (evt.key == mApp->getConfig().getIntValue(mApp->MoveLeft))
		mDirection.x = -1;
	else if (evt.key == mApp->getConfig().getIntValue(mApp->MoveRight))
		mDirection.x = 1;
	else if (evt.key == mApp->getConfig().getIntValue(mApp->Jump)) {
		if(mCharacter->isOnGround())
			mCharacter->addSpeed(Ogre::Vector3::UNIT_Y * 8);
	}
	else if (evt.key == OIS::KC_F5)
		toggleCameraMode();
}

void Player::injectKeyUp(const OIS::KeyEvent& evt) {
	// On met à jour la direction du joueur
	if (evt.key == mApp->getConfig().getIntValue(mApp->MoveForward) && mDirection.z == -1)
		mDirection.z = 0;
	else if (evt.key == mApp->getConfig().getIntValue(mApp->MoveLeft) && mDirection.x == -1)
		mDirection.x = 0;
	else if (evt.key == mApp->getConfig().getIntValue(mApp->MoveBackward) && mDirection.z == 1)
		mDirection.z = 0;
	else if (evt.key == mApp->getConfig().getIntValue(mApp->MoveRight) && mDirection.x == 1)
		mDirection.x = 0;
}

void Player::injectMouseMove(const OIS::MouseEvent& evt) {
	// On met à jour le point visé
	mCharacter->getNode()->yaw(-0.2 * Ogre::Degree(evt.state.X.rel));
	Ogre::Radian pitch = mCameraRootNode->getOrientation().getPitch();
	Ogre::Radian deltaPitch = -0.2*Ogre::Degree(evt.state.Y.rel);
	if(pitch + deltaPitch > Ogre::Degree(80))
		deltaPitch =  Ogre::Degree(80) - pitch;
	else if (pitch + deltaPitch < Ogre::Degree(-80))
		deltaPitch = Ogre::Degree(-80) - pitch;
	mCameraRootNode->pitch(deltaPitch);
}

void Player::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {

    Ogre::Vector3 faceVector;
	Block* target = NULL;
	if (mCharacter->getObjectManager()->blockReached(mCameraFPNode->_getDerivedPosition(), mCamera->getDerivedDirection(), 3, target, &faceVector)) {

	    if (id == OIS::MB_Left) {
            target->setIntegrity(target->getIntegrity()-60); //On abîme le bloc
		}
        else if (id == OIS::MB_Right)
        {
        	mCharacter->getObjectManager()->createBlock(target->getPosition()+faceVector);
        }
	}
}

void Player::takeADecision() {
	mCharacter->setPropulsionLocalDirection(mDirection);
}

void Player::setupCamera() {
	// On créer les noeuds pour les cameras
	mCameraRootNode = mCharacter->getNode()->getCreator()->createSceneNode();
	mCameraFPNode = mCameraRootNode->createChildSceneNode();
	mCameraTPNode = mCameraRootNode->createChildSceneNode(PhysicalObject::LocalDirectionBackward * 8);
	mCameraGoal = mCameraRootNode->createChildSceneNode(PhysicalObject::LocalDirectionForward * 10);
	mCamera->setAutoTracking(true,mCameraGoal);
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(mApp->getConfig().getIntValue(OgreApplication::FarClipDistance));
	mCamera->setFOVy(Ogre::Degree(70));
	// On attache la caméra en FPS par défaut
	mCameraFPNode->attachObject(mCamera);
}

void Player::toggleCameraMode() {
	// From First Person to Third Person
	if (mCamera->getParentSceneNode() == mCameraFPNode) {
#ifdef _WINDOWS
        mCamera->getParentSceneNode()->detachObject(mCamera);
#else
        mCamera->detachFromParent();
#endif

		mCameraTPNode->attachObject(mCamera);
		mCharacter->getEntity()->setVisible(true);
	}
	else { // From Third Person to First Person
#ifdef _WINDOWS
        mCamera->getParentSceneNode()->detachObject(mCamera);
#else
        mCamera->detachFromParent();
#endif
		mCameraFPNode->attachObject(mCamera);
		mCharacter->getEntity()->setVisible(false);
	}
}

void Player::setCharacter(AnimatedObject* character) {
	cleanCharacter();
	setupCharacter(character);
}

void Player::objectDied(const PhysicalObject* object) {
	mApp->requestEndGame();
}

void Player::setupCharacter(AnimatedObject* character) {
#ifdef DEBUG_MODE
LOG("enter Player::setupCharacter");
#endif
	mCharacter = character;
	mOldIA = character->getIA();
	mCharacter->setIA(this, false);
	mCharacter->getNode()->addChild(mCameraRootNode);
	// On place le noeud des cameras au niveau des yeux
	mCameraRootNode->setPosition(0, mCharacter->getVolume().y * 0.95, 0);
	mCharacter->addListener(this);
	if (mCamera->getParentSceneNode() == mCameraFPNode)
		mCharacter->getEntity()->setVisible(false);
#ifdef DEBUG_MODE
LOG("exit Player::setupCharacter");
#endif
}

void Player::cleanCharacter() {
#ifdef DEBUG_MODE
LOG("enter Player::cleanCharacter");
#endif
	mCharacter->removeListener(this);
	mCharacter->getNode()->removeChild(mCameraRootNode);
	mCharacter->setIA(mOldIA, false);
	mCharacter->getEntity()->setVisible(true);
#ifdef DEBUG_MODE
LOG("exit Player::cleanCharacter");
#endif
}

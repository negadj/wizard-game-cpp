/*
 * Player.cpp
 *
 *  Created on: 24 oct. 2011
 *      Author: gecko
 */

#include "Player.h"
#include "../ObjectManager.h"

#ifdef _WINDOWS
#define MOVE_FORWARD OIS::KC_W
#define MOVE_LEFT OIS::KC_A
#else
#define MOVE_FORWARD OIS::KC_Z
#define MOVE_LEFT OIS::KC_Q
#endif
#define MOVE_RIGHT OIS::KC_D
#define MOVE_BACKWARD OIS::KC_S

Player::Player(ObjectManager* objectManager, Ogre::String name, Camera* cam) :
	PhysicalObject(objectManager, cam->getSceneManager()->getRootSceneNode(), name, 1, "Sinbad.mesh", Ogre::Vector3(0.45,0.9,0.45),"Joueur"),
	mCamera(cam),
	mBodyNode(0),
	mCameraRootNode(0),
	mCameraGoal(0),
	mCameraFPNode(0),
	mCameraTPNode(0),
	mSword1(0),
	mSword2(0),
	mDirection(Ogre::Vector3::ZERO),
	mPropulsion(40),
	mVerticalVelocity(0),
	mCollisionTools(cam->getSceneManager())
{
	getNode()->translate(1,1.5,1);
	getNode()->setInitialState();
	setupBody(cam->getSceneManager());
	setupCamera();
	getNode()->yaw(Degree(180));
}

Player::~Player() {}

void Player::update(Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter Player::update");
#endif
	/* Mise à jour de la vitesse du joueur en fonction des touches,
	 dans le référentiel global. */
	if(isOnGround())
		addSpeed(deltaTime * (-getSpeed()*getObjectManager()->getStrench(this) + getObjectManager()->getGravity(this)+  mPropulsion * (getNode()->getOrientation() * mDirection.normalisedCopy())));
	else
		addSpeed(deltaTime * (-getSpeed()*getObjectManager()->getStrench(this) + getObjectManager()->getGravity(this)));

	if (mDirection != Ogre::Vector3::ZERO) {
		getEntity()->getAnimationState("RunBase")->setEnabled(true);
		getEntity()->getAnimationState("RunTop")->setEnabled(true);
		if (mDirection.z > 0) {
			getEntity()->getAnimationState("RunBase")->addTime(-deltaTime);
			getEntity()->getAnimationState("RunTop")->addTime(-deltaTime);
		}
		else {
			getEntity()->getAnimationState("RunBase")->addTime(deltaTime);
			getEntity()->getAnimationState("RunTop")->addTime(deltaTime);
		}
	}
	else {
		getEntity()->getAnimationState("RunBase")->setEnabled(false);
		getEntity()->getAnimationState("RunTop")->setEnabled(false);
		getEntity()->getAnimationState("RunBase")->setTimePosition(0); //RAZ de l'animation
		getEntity()->getAnimationState("RunTop")->setTimePosition(0); //RAZ de l'animation
	}

#ifdef DEBUG_MODE
LOG("exit Player::update");
#endif
}

void Player::injectKeyDown(const OIS::KeyEvent& evt) {
	switch (evt.key) {
	case OIS::KC_F5: //Change le mode de la camera
		toggleCameraMode();
		break;
	// On met à jour la direction du joueur
	case MOVE_FORWARD:
		mDirection.z = -1;
		break;
	case MOVE_LEFT:
		mDirection.x = -1;
		break;
	case MOVE_BACKWARD:
		mDirection.z = 1;
		break;
	case MOVE_RIGHT:
		mDirection.x = 1;
		break;
	case OIS::KC_SPACE:
		if(isOnGround())
			addSpeed(Ogre::Vector3::UNIT_Y * 15);
		break;
	default:
		break;
	}
}

void Player::injectKeyUp(const OIS::KeyEvent& evt) {
	// On met à jour la direction du joueur
	if (evt.key == MOVE_FORWARD && mDirection.z == -1)
		mDirection.z = 0;
	else if (evt.key == MOVE_LEFT && mDirection.x == -1)
		mDirection.x = 0;
	else if (evt.key == MOVE_BACKWARD && mDirection.z == 1)
		mDirection.z = 0;
	else if (evt.key == MOVE_RIGHT && mDirection.x == 1)
		mDirection.x = 0;
}

void Player::injectMouseMove(const OIS::MouseEvent& evt) {
	// On met à jour le point visé
	getNode()->yaw(-0.2*Degree(evt.state.X.rel));
	Radian pitch = mCameraRootNode->_getDerivedOrientation().getPitch();
	Radian deltaPitch = -0.2*Degree(evt.state.Y.rel);
	if(pitch + deltaPitch > Degree(60))
		deltaPitch =  Degree(60) - pitch;
	else if (pitch + deltaPitch < Degree(-60))
		deltaPitch = Degree(-60) - pitch;
	mCameraRootNode->pitch(deltaPitch);
}

void Player::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {


	Block* target = NULL;
	if (getObjectManager()->blockReached(mCamera->getDerivedPosition(), mCamera->getDerivedDirection(), 3, target)) {

	    if (id == 0 or id == 2)
            {getObjectManager()->getTerrain().removeBlock(target->getNode()->getPosition());}
        else if (id == 1)
           {Block* newBlock = getObjectManager()->createBlock(target->getNode()->getPosition()+Ogre::Vector3(0,1,0));
           getObjectManager()->getTerrain().addBlock(*newBlock);}
	}
}

void Player::setupBody(SceneManager* sceneMgr) {
	// Entités pour le corps
	mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	getEntity()->attachObjectToBone("Sheath.L", mSword1);
	getEntity()->attachObjectToBone("Sheath.R", mSword2);

	// Création du corps du personnage
	mBodyNode = getNode()->createChildSceneNode(Vector3::UNIT_Y * 0.05);
	mBodyNode->scale(Vector3::UNIT_SCALE * 0.2);
	mBodyNode->yaw(Degree(180));
	mBodyNode->setInitialState();
	getEntity()->getAnimationState("RunBase")->setEnabled(true);

	/* Le constructeur de PhysicalObject attache mEntity à mNode,
	   ce n'est pas ce que l'on souhaite ici */
#ifdef _WINDOWS
    getEntity()->getParentSceneNode()->detachObject(getEntity());
#else
    getEntity()->detachFromParent();
#endif

	mBodyNode->attachObject(getEntity());
}

void Player::setupCamera() {
	// On créer les noeuds pour les cameras
	mCameraRootNode = getNode()->createChildSceneNode(Vector3(0,0.3,-0.3));
	mCameraFPNode = mCameraRootNode->createChildSceneNode();
	mCameraTPNode = mCameraRootNode->createChildSceneNode(Vector3::UNIT_Z*8);
	mCameraGoal = mCameraRootNode->createChildSceneNode(Vector3::NEGATIVE_UNIT_Z*10);
	mCamera->setAutoTracking(true,mCameraGoal);
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50);
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
	}
	else { // From Third Person to First Person
#ifdef _WINDOWS
        mCamera->getParentSceneNode()->detachObject(mCamera);
#else
        mCamera->detachFromParent();
#endif
		mCameraFPNode->attachObject(mCamera);
	}
}

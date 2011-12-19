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
	AnimatedObject(objectManager, cam->getSceneManager()->getRootSceneNode(), name, "Sinbad.mesh", Ogre::Vector3(0.45,0.9,0.45), -0.05, 0.2, Degree(180), "Joueur"),
	mCamera(cam),
	mCameraRootNode(0),
	mCameraGoal(0),
	mCameraFPNode(0),
	mCameraTPNode(0),
	mSword1(0),
	mSword2(0),
	mDirection(Ogre::Vector3::ZERO),
	mCollisionTools(cam->getSceneManager())
{
	registerType(TYPE_FRIENDLY);
	getNode()->translate(1,1.5,1);
	getNode()->setInitialState();
	mSword1 = cam->getSceneManager()->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = cam->getSceneManager()->createEntity("SinbadSword2", "Sword.mesh");
	getEntity()->attachObjectToBone("Sheath.L", mSword1);
	getEntity()->attachObjectToBone("Sheath.R", mSword2);
	setupCamera();
	registerAnimation("RunBase");
	registerAnimation("RunTop");
}

Player::~Player() {
	//getNode()->removeAndDestroyChild(mBodyNode->getName());
	getNode()->getCreator()->destroyEntity(mSword1);
	getNode()->getCreator()->destroyEntity(mSword2);
}

void Player::doPreCollisionUpdate(Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter Player::doPreCollisionUpdate");
#endif
	/* Mise à jour de la vitesse du joueur en fonction des touches,
	 dans le référentiel global. */
	if(isOnGround())
		/* Ajout de la force motrice */
		addForce((getPropulsion() * (getNode()->getOrientation() * mDirection.normalisedCopy())));
    else
        addForce((getPropulsion() * (getNode()->getOrientation() * mDirection.normalisedCopy()))/15);


	AnimatedObject::doPreCollisionUpdate(deltaTime);
#ifdef DEBUG_MODE
LOG("exit Player::doPreCollisionUpdate");
#endif
}

void Player::doPostCollisionUpdate(Ogre::Real deltaTime)
{
	if (getSpeed().isZeroLength()) {
		disableAnimation("RunTop");
		disableAnimation("RunBase");
	}
	else {
		enableAnimation("RunTop");
		enableAnimation("RunBase");
	}
	AnimatedObject::doPostCollisionUpdate(deltaTime);
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
			addSpeed(Ogre::Vector3::UNIT_Y * 8);
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
	Radian pitch = mCameraRootNode->getOrientation().getPitch();
	Radian deltaPitch = -0.2*Degree(evt.state.Y.rel);
	if(pitch + deltaPitch > Degree(80))
		deltaPitch =  Degree(80) - pitch;
	else if (pitch + deltaPitch < Degree(-80))
		deltaPitch = Degree(-80) - pitch;
	mCameraRootNode->pitch(deltaPitch);
}

void Player::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {

    Ogre::Vector3 faceVector;
	Block* target = NULL;
	if (getObjectManager()->blockReached(mCameraFPNode->_getDerivedPosition(), mCamera->getDerivedDirection(), 3, target, &faceVector)) {

	    if (id == OIS::MB_Left) {
            target->setIntegrity(target->getIntegrity()-60); //On abîme le bloc
		}
        else if (id == OIS::MB_Right)
        {
        	getObjectManager()->createBlock(target->getPosition()+faceVector);
        }
	}
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
	mCamera->setFOVy(Degree(75));
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

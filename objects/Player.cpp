/*
 * Player.cpp
 *
 *  Created on: 24 oct. 2011
 *      Author: gecko
 */

#include "Player.h"

//#define NUM_ANIMS 13           // number of animations the character has
#define CHAR_HEIGHT 2          // hauteur du centre de gravité du personnage
//#define CAM_HEIGHT 2           // height of camera above character's center of mass
//#define RUN_SPEED 17           // character running speed in units per second
//#define TURN_SPEED 500.0f      // character turning in degrees per second
//#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
//#define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second
//#define GRAVITY 90.0f          // gravity in downward units per squared second

Player::Player(Ogre::String name, Camera* cam) :
	PhysicalObject(cam->getSceneManager()->getRootSceneNode()->createChildSceneNode(), name, 1, "Sinbad.mesh", "Joueur"),
	mCamera(cam),
//	mPlayerNode(0),
	mBodyNode(0),
	mCameraRootNode(0),
	mCameraGoal(0),
	mCameraFPNode(0),
	mCameraTPNode(0),
//	mBodyEnt(0),
	mSword1(0),
	mSword2(0),
//	mCollisionMgr(0),
	mDirection(),
	mVelocity(5)
	{
	setupBody(cam->getSceneManager());
	setupCamera();
	}
//	mCollisionMgr = new MOC::CollisionTools(mCamera->getSceneManager());
//	setupBody(mCamera->getSceneManager());
//	setupCamera();
//	setupAnimations();
//}

Player::~Player() {
//	delete mCollisionMgr;
}

//void Player::setup() {
//	setupBody(mCamera->getSceneManager());
//	setupCamera();
//}

void Player::update(Real deltaTime) {
	/* Mise à jour de la vitesse du joueur en fonction des touches,
	 dans le référentiel global. */
	setSpeed(mVelocity * (getNode()->getOrientation() * mDirection).normalisedCopy());
//	Vector3 movement = mDirection.normalisedCopy()*(deltaTime * RUN_SPEED);
//	Vector3 oldPos = mPlayerNode->getPosition();
//	mPlayerNode->translate(movement, Node::TS_LOCAL);
//	Vector3 newPos = mPlayerNode->getPosition();
//	//On vérifie les collisions
//	if (mCollisionMgr->collidesWithEntity(oldPos,newPos, 0))
//		mPlayerNode->translate(-1*movement, Node::TS_LOCAL);
//	updateBody(deltaTime);
//	updateAnimations(deltaTime);
//	updateCamera(deltaTime);
}

void Player::injectKeyDown(const OIS::KeyEvent& evt) {
//	if (evt.key == OIS::KC_Q
//			&& (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)) {
//		// take swords out (or put them back, since it's the same animation but reversed)
//		setTopAnimation(ANIM_DRAW_SWORDS, true);
//		mTimer = 0;
//	} else if (evt.key == OIS::KC_E && !mSwordsDrawn) {
//		if (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP) {
//			// start dancing
//			setBaseAnimation(ANIM_DANCE, true);
//			setTopAnimation(ANIM_NONE);
//			// disable hand animation because the dance controls hands
//			mAnims[ANIM_HANDS_RELAXED]->setEnabled(false);
//		} else if (mBaseAnimID == ANIM_DANCE) {
//			// stop dancing
//			setBaseAnimation(ANIM_IDLE_BASE);
//			setTopAnimation(ANIM_IDLE_TOP);
//			// re-enable hand animation
//			mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
//		}
//	}

	switch (evt.key) {
	case OIS::KC_F5: //Change le mode de la camera
		toggleCameraMode();
		break;
	// On met à jour la direction du joueur
	case OIS::KC_Z:
		mDirection.z = -1;
		break;
	case OIS::KC_Q:
		mDirection.x = -1;
		break;
	case OIS::KC_S:
		mDirection.z = 1;
		break;
	case OIS::KC_D:
		mDirection.x = 1;
		break;
	default:
		break;
	}


//	else if (evt.key == OIS::KC_SPACE
//			&& (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)) {
//		// jump if on ground
//		setBaseAnimation(ANIM_JUMP_START, true);
//		setTopAnimation(ANIM_NONE);
//		mTimer = 0;
//	}
//
//	if (!mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_IDLE_BASE) {
//		// start running if not already moving and the player wants to move
//		setBaseAnimation(ANIM_RUN_BASE, true);
//		if (mTopAnimID == ANIM_IDLE_TOP)
//			setTopAnimation(ANIM_RUN_TOP, true);
//	}
}

void Player::injectKeyUp(const OIS::KeyEvent& evt) {
	// On met à jour la direction du joueur
	if (evt.key == OIS::KC_Z && mDirection.z == -1)
		mDirection.z = 0;
	else if (evt.key == OIS::KC_Q && mDirection.x == -1)
		mDirection.x = 0;
	else if (evt.key == OIS::KC_S && mDirection.z == 1)
		mDirection.z = 0;
	else if (evt.key == OIS::KC_D && mDirection.x == 1)
		mDirection.x = 0;

//	if (mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_RUN_BASE) {
//		// stop running if already moving and the player doesn't want to move
//		setBaseAnimation(ANIM_IDLE_BASE);
//		if (mTopAnimID == ANIM_RUN_TOP)
//			setTopAnimation(ANIM_IDLE_TOP);
//	}
}

void Player::injectMouseMove(const OIS::MouseEvent& evt) {
	// On met à jour le point de visée
	getNode()->yaw(-0.2*Degree(evt.state.X.rel));
	Radian pitch = mCameraRootNode->_getDerivedOrientation().getPitch();
	Radian deltaPitch = -0.2*Degree(evt.state.Y.rel);
	if (pitch + deltaPitch < Degree(60) && pitch + deltaPitch > Degree(-60))
		mCameraRootNode->pitch(deltaPitch);
}

void Player::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {
//	if (mSwordsDrawn
//			&& (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)) {
//		// if swords are out, and character's not doing something weird, then SLICE!
//		if (id == OIS::MB_Left)
//			setTopAnimation(ANIM_SLICE_VERTICAL, true);
//		else if (id == OIS::MB_Right)
//			setTopAnimation(ANIM_SLICE_HORIZONTAL, true);
//		mTimer = 0;
//	}
}

void Player::setupBody(SceneManager* sceneMgr) {
	// Entités pour le corps
//	setEntity(sceneMgr->createEntity("SinbadBody", "Sinbad.mesh"));
	mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	getEntity()->attachObjectToBone("Sheath.L", mSword1);
	getEntity()->attachObjectToBone("Sheath.R", mSword2);

	// Création du corps du personnage
	mBodyNode = getNode()->createChildSceneNode(Vector3::UNIT_Y * (CHAR_HEIGHT / 2));
	mBodyNode->scale(Vector3::UNIT_SCALE * 0.2);
	mBodyNode->yaw(Degree(180));
	mBodyNode->setInitialState();

	/* Le constructeur de PhysicalObject attache mEntity à mNode,
	   ce n'est pas ce que l'on souhaite ici */
	getEntity()->detachFromParent();
	mBodyNode->attachObject(getEntity());

	// create a couple of ribbon trails for the swords, just for fun
//	NameValuePairList params;
//	params["numberOfChains"] = "2";
//	params["maxElements"] = "80";
//	mSwordTrail = (RibbonTrail*) sceneMgr->createMovableObject("RibbonTrail",
//			&params);
//	mSwordTrail->setMaterialName("Examples/LightRibbonTrail");
//	mSwordTrail->setTrailLength(20);
//	mSwordTrail->setVisible(false);
//	sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);
//
//	for (int i = 0; i < 2; i++) {
//		mSwordTrail->setInitialColour(i, 1, 0.8, 0);
//		mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
//		mSwordTrail->setWidthChange(i, 1);
//		mSwordTrail->setInitialWidth(i, 0.5);
//	}

	mDirection = Vector3::ZERO;
	mVerticalVelocity = 0;
}

void Player::setupCamera() {
	// On créer les noeuds pour les cameras
	mCameraRootNode = getNode()->createChildSceneNode(Vector3(0,CHAR_HEIGHT,-0.2));
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
		mCamera->detachFromParent();
		mCameraTPNode->attachObject(mCamera);
	}
	else { // From Third Person to First Person
		mCamera->detachFromParent();
		mCameraFPNode->attachObject(mCamera);
	}
}

//void Player::setupAnimations() {
//	// this is very important due to the nature of the exported animations
//	mBodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);
//
//	String animNames[] = { "IdleBase", "IdleTop", "RunBase", "RunTop",
//			"HandsClosed", "HandsRelaxed", "DrawSwords", "SliceVertical",
//			"SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd" };
//
//	// populate our animation list
//	for (int i = 0; i < NUM_ANIMS; i++) {
//		mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
//		mAnims[i]->setLoop(true);
//		mFadingIn[i] = false;
//		mFadingOut[i] = false;
//	}
//
//	// start off in the idle state (top and bottom together)
//	setBaseAnimation(ANIM_IDLE_BASE);
//	setTopAnimation(ANIM_IDLE_TOP);
//
//	// relax the hands since we're not holding anything
//	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
//
//	mSwordsDrawn = false;
//}
//
//void Player::updateBody(Real deltaTime) {
//	mGoalDirection = Vector3::ZERO; // we will calculate this
//
//	if (mKeyDirection != Vector3::ZERO && mBaseAnimID != ANIM_DANCE) {
//		// calculate actually goal direction in world based on player's key directions
//		mGoalDirection += mKeyDirection.z
//				* mCameraNode->getOrientation().zAxis();
//		mGoalDirection += mKeyDirection.x
//				* mCameraNode->getOrientation().xAxis();
//		mGoalDirection.y = 0;
//		mGoalDirection.normalise();
//
//		Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(
//				mGoalDirection);
//
//		// calculate how much the character has to turn to face goal direction
//		Real yawToGoal = toGoal.getYaw().valueDegrees();
//		// this is how much the character CAN turn this frame
//		Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime
//				* TURN_SPEED;
//		// reduce "turnability" if we're in midair
//		if (mBaseAnimID == ANIM_JUMP_LOOP)
//			yawAtSpeed *= 0.2f;
//
//		// turn as much as we can, but not more than we need to
//		if (yawToGoal < 0)
//			yawToGoal = std::min<Real>(0,
//					std::max<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
//		else if (yawToGoal > 0)
//			yawToGoal = std::max<Real>(0,
//					std::min<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);
//
//		mBodyNode->yaw(Degree(yawToGoal));
//
//		// move in current body direction (not the goal direction)
//		mBodyNode->translate(0, 0,
//				deltaTime * RUN_SPEED * mAnims[mBaseAnimID]->getWeight(),
//				Node::TS_LOCAL);
//	}
//
//	if (mBaseAnimID == ANIM_JUMP_LOOP) {
//		// if we're jumping, add a vertical offset too, and apply gravity
//		mBodyNode->translate(0, mVerticalVelocity * deltaTime, 0,
//				Node::TS_LOCAL);
//		mVerticalVelocity -= GRAVITY * deltaTime;
//
//		Vector3 pos = mBodyNode->getPosition();
//		if (pos.y <= CHAR_HEIGHT)
//		{
//			// if we've hit the ground, change to landing state
//			pos.y = CHAR_HEIGHT;
//			mBodyNode->setPosition(pos);
//			setBaseAnimation(ANIM_JUMP_END, true);
//			mTimer = 0;
//		}
//	}
//}

//void Player::updateAnimations(Real deltaTime) {
//	Real baseAnimSpeed = 1;
//	Real topAnimSpeed = 1;
//
//	mTimer += deltaTime;
//
//	if (mTopAnimID == ANIM_DRAW_SWORDS) {
//		// flip the draw swords animation if we need to put it back
//		topAnimSpeed = mSwordsDrawn ? -1 : 1;
//
//		// half-way through the animation is when the hand grasps the handles...
//		if (mTimer >= mAnims[mTopAnimID]->getLength() / 2
//				&& mTimer - deltaTime < mAnims[mTopAnimID]->getLength() / 2) {
//			// so transfer the swords from the sheaths to the hands
//			mBodyEnt->detachAllObjectsFromBone();
//			mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L",
//					mSword1);
//			mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R",
//					mSword2);
//			// change the hand state to grab or let go
//			mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
//			mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);
//
//			// toggle sword trails
//			if (mSwordsDrawn) {
//				mSwordTrail->setVisible(false);
//				mSwordTrail->removeNode(mSword1->getParentNode());
//				mSwordTrail->removeNode(mSword2->getParentNode());
//			} else {
//				mSwordTrail->setVisible(true);
//				mSwordTrail->addNode(mSword1->getParentNode());
//				mSwordTrail->addNode(mSword2->getParentNode());
//			}
//		}
//
//		if (mTimer >= mAnims[mTopAnimID]->getLength()) {
//			// animation is finished, so return to what we were doing before
//			if (mBaseAnimID == ANIM_IDLE_BASE)
//				setTopAnimation(ANIM_IDLE_TOP);
//			else {
//				setTopAnimation(ANIM_RUN_TOP);
//				mAnims[ANIM_RUN_TOP]->setTimePosition(
//						mAnims[ANIM_RUN_BASE]->getTimePosition());
//			}
//			mSwordsDrawn = !mSwordsDrawn;
//		}
//	} else if (mTopAnimID == ANIM_SLICE_VERTICAL
//			|| mTopAnimID == ANIM_SLICE_HORIZONTAL) {
//		if (mTimer >= mAnims[mTopAnimID]->getLength()) {
//			// animation is finished, so return to what we were doing before
//			if (mBaseAnimID == ANIM_IDLE_BASE)
//				setTopAnimation(ANIM_IDLE_TOP);
//			else {
//				setTopAnimation(ANIM_RUN_TOP);
//				mAnims[ANIM_RUN_TOP]->setTimePosition(
//						mAnims[ANIM_RUN_BASE]->getTimePosition());
//			}
//		}
//
//		// don't sway hips from side to side when slicing. that's just embarrasing.
//		if (mBaseAnimID == ANIM_IDLE_BASE)
//			baseAnimSpeed = 0;
//	} else if (mBaseAnimID == ANIM_JUMP_START) {
//		if (mTimer >= mAnims[mBaseAnimID]->getLength()) {
//			// takeoff animation finished, so time to leave the ground!
//			setBaseAnimation(ANIM_JUMP_LOOP, true);
//			// apply a jump acceleration to the character
//			mVerticalVelocity = JUMP_ACCEL;
//		}
//	} else if (mBaseAnimID == ANIM_JUMP_END) {
//		if (mTimer >= mAnims[mBaseAnimID]->getLength()) {
//			// safely landed, so go back to running or idling
//			if (mKeyDirection == Vector3::ZERO) {
//				setBaseAnimation(ANIM_IDLE_BASE);
//				setTopAnimation(ANIM_IDLE_TOP);
//			} else {
//				setBaseAnimation(ANIM_RUN_BASE, true);
//				setTopAnimation(ANIM_RUN_TOP, true);
//			}
//		}
//	}
//
//	// increment the current base and top animation times
//	if (mBaseAnimID != ANIM_NONE)
//		mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
//	if (mTopAnimID != ANIM_NONE)
//		mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);
//
//	// apply smooth transitioning between our animations
//	fadeAnimations(deltaTime);
//}

//void Player::fadeAnimations(Real deltaTime) {
//	for (int i = 0; i < NUM_ANIMS; i++) {
//		if (mFadingIn[i]) {
//			// slowly fade this animation in until it has full weight
//			Real newWeight = mAnims[i]->getWeight()
//					+ deltaTime * ANIM_FADE_SPEED;
//			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
//			if (newWeight >= 1)
//				mFadingIn[i] = false;
//		} else if (mFadingOut[i]) {
//			// slowly fade this animation out until it has no weight, and then disable it
//			Real newWeight = mAnims[i]->getWeight()
//					- deltaTime * ANIM_FADE_SPEED;
//			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
//			if (newWeight <= 0) {
//				mAnims[i]->setEnabled(false);
//				mFadingOut[i] = false;
//			}
//		}
//	}
//}

//void Player::updateCamera(Real deltaTime) {
//	// place the camera pivot roughly at the character's shoulder
//	mCameraPivot->setPosition(
//			mBodyNode->getPosition() + Vector3::UNIT_Y * CAM_HEIGHT);
//	// move the camera smoothly to the goal
//	Vector3 goalOffset = mCameraGoal->_getDerivedPosition()
//			- mCameraNode->getPosition();
//	mCameraNode->translate(goalOffset * deltaTime * 9.0f);
//	// always look at the pivot
//	mCameraNode->lookAt(mCameraPivot->_getDerivedPosition(), Node::TS_WORLD);
//}
//
//void Player::updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom) {
//	mCameraPivot->yaw(Degree(deltaYaw), Node::TS_WORLD);
//
//	// bound the pitch
//	if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0)
//			&& !(mPivotPitch + deltaPitch < -60 && deltaPitch < 0)) {
//		mCameraPivot->pitch(Degree(deltaPitch), Node::TS_LOCAL);
//		mPivotPitch += deltaPitch;
//	}
//
//	Real dist = mCameraGoal->_getDerivedPosition().distance(
//			mCameraPivot->_getDerivedPosition());
//	Real distChange = deltaZoom * dist;
//
//	// bound the zoom
//	if (!(dist + distChange < 8 && distChange < 0)
//			&& !(dist + distChange > 25 && distChange > 0)) {
//		mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
//	}
//}
//
//void Player::setBaseAnimation(AnimID id, bool reset) {
//	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
//	{
//		// if we have an old animation, fade it out
//		mFadingIn[mBaseAnimID] = false;
//		mFadingOut[mBaseAnimID] = true;
//	}
//
//	mBaseAnimID = id;
//
//	if (id != ANIM_NONE) {
//		// if we have a new animation, enable it and fade it in
//		mAnims[id]->setEnabled(true);
//		mAnims[id]->setWeight(0);
//		mFadingOut[id] = false;
//		mFadingIn[id] = true;
//		if (reset)
//			mAnims[id]->setTimePosition(0);
//	}
//}
//
//void Player::setTopAnimation(AnimID id, bool reset) {
//	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
//	{
//		// if we have an old animation, fade it out
//		mFadingIn[mTopAnimID] = false;
//		mFadingOut[mTopAnimID] = true;
//	}
//
//	mTopAnimID = id;
//
//	if (id != ANIM_NONE) {
//		// if we have a new animation, enable it and fade it in
//		mAnims[id]->setEnabled(true);
//		mAnims[id]->setWeight(0);
//		mFadingOut[id] = false;
//		mFadingIn[id] = true;
//		if (reset)
//			mAnims[id]->setTimePosition(0);
//	}
//}

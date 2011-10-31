/*
 * Player.h
 *
 *  Created on: 24 oct. 2011
 *      Author: gecko
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#define NUM_ANIMS 13           // number of animations the character has

#include <Ogre.h>
#include <OIS.h>
#include "PhysicalObject.h"
//#include "CollisionTools.h"

using namespace Ogre;

class Player : public PhysicalObject {
public:
	Player(Camera* cam);
	virtual ~Player();
	void update(Real deltaTime);
	void injectKeyDown(const OIS::KeyEvent& evt);
	void injectKeyUp(const OIS::KeyEvent& evt);
	void injectMouseMove(const OIS::MouseEvent& evt);
	void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	/*
	 * Change the camera mode (First Person or Third Person)
	 */
	void toggleCameraMode();

private:
	// all the animations our character has, and a null ID
	// some of these affect separate body parts and will be blended together
//	enum AnimID {
//		ANIM_IDLE_BASE,
//		ANIM_IDLE_TOP,
//		ANIM_RUN_BASE,
//		ANIM_RUN_TOP,
//		ANIM_HANDS_CLOSED,
//		ANIM_HANDS_RELAXED,
//		ANIM_DRAW_SWORDS,
//		ANIM_SLICE_VERTICAL,
//		ANIM_SLICE_HORIZONTAL,
//		ANIM_DANCE,
//		ANIM_JUMP_START,
//		ANIM_JUMP_LOOP,
//		ANIM_JUMP_END,
//		ANIM_NONE
//	};

	Camera* mCamera;
//	SceneNode* mPlayerNode;
	SceneNode* mBodyNode;
	SceneNode* mCameraRootNode;
	SceneNode* mCameraGoal;
	SceneNode* mCameraFPNode;
	SceneNode* mCameraTPNode;
//	Real mPivotPitch;
	Entity* mBodyEnt;
	Entity* mSword1;
	Entity* mSword2;
//	MOC::CollisionTools* mCollisionMgr;
//	RibbonTrail* mSwordTrail;
//	AnimationState* mAnims[NUM_ANIMS]; // master animation list
//	AnimID mBaseAnimID; // current base (full- or lower-body) animation
//	AnimID mTopAnimID; // current top (upper-body) animation
//	bool mFadingIn[NUM_ANIMS]; // which animations are fading in
//	bool mFadingOut[NUM_ANIMS]; // which animations are fading out
//	bool mSwordsDrawn;
	Vector3 mDirection; // player's local intended direction based on WASD keys
//	Vector3 mGoalDirection; // actual intended direction in world-space
	Real mVerticalVelocity; // for jumping
	Real mVelocity;
//	Real mTimer; // general timer to see how long animations have been playing

	virtual void setup();
	void setupBody(SceneManager* sceneMgr);
//	void setupAnimations();
	void setupCamera();
//	void updateBody(Real deltaTime);
//	void updateAnimations(Real deltaTime);
//	void fadeAnimations(Real deltaTime);
//	void updateCamera(Real deltaTime);
//	void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);
//	void setBaseAnimation(AnimID id, bool reset = false);
//	void setTopAnimation(AnimID id, bool reset = false);
};

#endif /* PLAYER_H_ */

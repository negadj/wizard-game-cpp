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
#include "../CollisionTools.h"

using namespace Ogre;

class Player : public PhysicalObject {
	friend class ObjectManager;
public:
	void update(Real deltaTime);
	void injectKeyDown(const OIS::KeyEvent& evt);
	void injectKeyUp(const OIS::KeyEvent& evt);
	void injectMouseMove(const OIS::MouseEvent& evt);
	void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	/*
	 * Change the camera mode (First Person or Third Person)
	 */
	void toggleCameraMode();

protected:
	Player(ObjectManager* objectManager, Ogre::String name, Camera* cam);
	virtual ~Player();

private:
	Camera* mCamera;
	SceneNode* mBodyNode;
	SceneNode* mCameraRootNode;
	SceneNode* mCameraGoal;
	SceneNode* mCameraFPNode;
	SceneNode* mCameraTPNode;
	Entity* mSword1;
	Entity* mSword2;
	Vector3 mDirection; // direction du joueur donnée par les touches du clavier
	Real mPropulsion;
	Real mVerticalVelocity; // for jumping
	MOC::CollisionTools mCollisionTools;

	void setupBody(SceneManager* sceneMgr);
	void setupCamera();
};

#endif /* PLAYER_H_ */

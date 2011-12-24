/*
 * Player.h
 *
 *  Created on: 24 oct. 2011
 *      Author: gecko
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <Ogre.h>
#include <OIS.h>
#include "IA/IA.h"
#include "objects/PhysicalObjectListener.h"

class OgreApplication;

/*
 * Represente un joueur, pas son personnage
 */
class Player : public IA, public PhysicalObjectListener {
public:
	Player(OgreApplication* app, AnimatedObject* character, Ogre::Camera* cam);
	virtual ~Player();
	void injectKeyDown(const OIS::KeyEvent& evt);
	void injectKeyUp(const OIS::KeyEvent& evt);
	void injectMouseMove(const OIS::MouseEvent& evt);
	void injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	/*
	 * Change the camera mode (First Person or Third Person)
	 */
	void toggleCameraMode();
	virtual void takeADecision();
	void setCharacter(AnimatedObject* character);

	/* Méthode héritée de l'interface PhysicalObjectListener.
	 * Nécessaire pour déterminer la fin de la partie.
	 */
	virtual void objectDied(const PhysicalObject* object);

private:
	OgreApplication* mApp;
	AnimatedObject* mCharacter;
	IA* mOldIA; // Ancienne IA de l'objet contrôlé
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mCameraRootNode;
	Ogre::SceneNode* mCameraGoal;
	Ogre::SceneNode* mCameraFPNode;
	Ogre::SceneNode* mCameraTPNode;
	/* direction du joueur donnée par les touches du clavier
	 * On utilise cet attribut plutot que la seule manipulation de la
	 * direction de propulsion du personnage par soucis de simplicité de code.
	 */
	Ogre::Vector3 mDirection;

	void setupCamera();
	void setupCharacter(AnimatedObject* character);
	void cleanCharacter();
};

#endif /* PLAYER_H_ */

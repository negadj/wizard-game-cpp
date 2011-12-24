/*
Author: Gecko
*/
#ifndef __OGREAPPLICATION_H_
#define __OGREAPPLICATION_H_

#include <Ogre.h>
#include <OIS.h>
#include "ObjectManager.h"
#include "MenuManager.h"
#include "objects/PhysicalObjectListener.h"
#include "Config.h"
#include "Player.h"

class OgreApplication : public Ogre::FrameListener, public Ogre::WindowEventListener,
public OIS::MouseListener, public OIS::KeyListener {
friend class MenuManager;

public:
	OgreApplication();
	virtual ~OgreApplication();
	bool start();
	Config& getConfig();
	void requestEndGame();

	static const Ogre::String MoveForward;
	static const Ogre::String MoveBackward;
	static const Ogre::String MoveLeft;
	static const Ogre::String MoveRight;
	static const Ogre::String Jump;
	static const Ogre::String Action1;
	static const Ogre::String Action2;
	static const Ogre::String Menu;
	static const Ogre::String FarClipDistance;

private:
	void loadResources();
	/*
	 * Associe des valeurs par défaut aux clés non répertoriées dans le fichier de conf.
	 */
	void loadDefaultConfig();
	void createScene();
	void createCamera();
	void createViewPort();
	void createTrayManager();
	void createFrameListener();
	void setupGUI();
	/**
	 * Initialise les ressources pour une partie
	 */
	void startGame();
	/**
	 * Libère les ressources de la partie
	 */
	void exitGame();

#ifdef _WINDOWS
    bool OgreApplication::frameStarted(const FrameEvent& evt);
#else
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
#endif
	void startOIS();
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	void toggleDebugOverlay();
	void updateDebugInfo(Ogre::Real deltaTime);

	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewPort;

	OIS::InputManager*  mInputManager;
	OIS::Mouse*         mMouse;
	OIS::Keyboard*      mKeyboard;

	ObjectManager* mObjectMgr;
    MenuManager mMenuMgr;
    Ogre::Overlay* mDebugOverlay;

	Player* mPlayer;

	Config mConfig;
	bool mContinue; // le programme tourne tant que mContinue est vrai
	bool mStarted; // indique si une partie est en cours
	bool mLocked; // indique si les commandes du joueur sont bloquées (pour les menus par ex.)
	/*
	 * indique si la partie doit se terminer. Une telle variable permet d'attendre la fin d'une boucle
	 * de rendu avant de terminer la partie et nettoyer la scène. Cela évite un crash dû au fait que
	 * la pile de méthodes qui déclenche la fin de partie provient d'un objet de la scène elle-même.
	 */
	bool mRequestEndGame;

};

#endif

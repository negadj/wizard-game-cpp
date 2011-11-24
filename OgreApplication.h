/*
Author: Gecko
*/
#ifndef __OGREAPPLICATION_H_
#define __OGREAPPLICATION_H_

#include <Ogre.h>
#include <OIS.h>
#include "objects/ObjectManager.h"
#ifndef NO_CEGUI
#include "MenuManager.h"
#endif

using namespace Ogre;

class OgreApplication : public FrameListener, public WindowEventListener,
public OIS::MouseListener, public OIS::KeyListener {
friend class MenuManager;

public:
	OgreApplication();
	virtual ~OgreApplication();
	bool start();

private:
	void loadResources();
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
	void windowResized(RenderWindow* rw);
	void windowClosed(RenderWindow* rw);

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

	void toggleDebugOverlay();
	void updateDebugInfo(Real deltaTime);

	Root* mRoot;
	RenderWindow* mWindow;
	SceneManager* mSceneMgr;
	Camera* mCamera;
	Viewport* mViewPort;
	ObjectManager* mObjectMgr;

	OIS::InputManager*  mInputManager;
	OIS::Mouse*         mMouse;
	OIS::Keyboard*      mKeyboard;

#ifndef NO_CEGUI
    MenuManager mMenuMgr;
#endif
	Overlay* mDebugOverlay;

	Player* mPlayer;
	bool mContinue; // le programme tourne tant que mContinue est vrai
	bool mStarted; // indique si une partie est en cours
	bool mLocked; // indique si les commandes du joueur sont bloquées (pour les menus par ex.)
};

#endif

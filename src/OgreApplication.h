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

using namespace Ogre;

class OgreApplication : public FrameListener, public WindowEventListener,
public OIS::MouseListener, public OIS::KeyListener, public PhysicalObjectListener {
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

	/* Méthodes héritées de l'interface PhysicalObjectListener.
	 * Nécessaire pour déterminer la fin de la partie.
	 */
	virtual void objectDied(const PhysicalObject* object);

	Root* mRoot;
	RenderWindow* mWindow;
	SceneManager* mSceneMgr;
	Camera* mCamera;
	Viewport* mViewPort;

	OIS::InputManager*  mInputManager;
	OIS::Mouse*         mMouse;
	OIS::Keyboard*      mKeyboard;

	ObjectManager* mObjectMgr;
    MenuManager mMenuMgr;
	Overlay* mDebugOverlay;

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

/*
Author: Gecko
*/
#ifndef __OGREAPPLICATION_H_
#define __OGREAPPLICATION_H_

#include <Ogre.h>
#include <OIS.h>
#include "objects/ObjectManager.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"

using namespace Ogre;

class OgreApplication : public FrameListener, public WindowEventListener,
public OIS::MouseListener, public OIS::KeyListener {
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

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
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
	void toggleMenu();
	bool quit(const CEGUI::EventArgs &e);

	Root* mRoot;
	RenderWindow* mWindow;
	SceneManager* mSceneMgr;
	Camera* mCamera;
	Viewport* mViewPort;
	ObjectManager* mObjectMgr;

	OIS::InputManager*  mInputManager;
	OIS::Mouse*         mMouse;
	OIS::Keyboard*      mKeyboard;

	Overlay* mDebugOverlay;
	CEGUI::OgreRenderer* mCeguiRenderer;
	CEGUI::Window* mMenuSheet;

	Player* mPlayer;
	bool mContinue;
	bool mLocked;
};

#endif

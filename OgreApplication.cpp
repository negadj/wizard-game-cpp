/*
Author: Gecko
*/
#include "OgreApplication.h"
#include "objects/Player.h"

OgreApplication::OgreApplication() :
	mRoot(NULL),
	mWindow(NULL),
	mSceneMgr(NULL),
	mCamera(NULL),
	mViewPort(NULL),
	mObjectMgr(NULL),
	mInputManager(NULL),
	mMouse(NULL),
	mKeyboard(NULL),
	mMenuMgr(this),
	mDebugOverlay(NULL),
	mPlayer(NULL),
	mContinue(true),
	mStarted(false),
	mLocked(true)
{}

OgreApplication::~OgreApplication() {
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mObjectMgr;
	delete mRoot;
}

bool OgreApplication::start() {
	mRoot = new Root("config/plugins.cfg", "config/ogre.cfg", "log/Wizard.log");

	loadResources();

	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	    return false;

	mWindow = mRoot->initialise(true, "Wizard");
	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "Wizard Scene Manager");
	mDebugOverlay = OverlayManager::getSingleton().getByName("Wizard/DebugOverlay");
	createCamera();
	createViewPort();
	mMenuMgr.setup();
	createFrameListener();

	mMenuMgr.showMainMenu();
	mRoot->startRendering();

	return true;
}

void OgreApplication::startGame() {
	mObjectMgr = new ObjectManager(mSceneMgr);
	createScene();
	mPlayer = mObjectMgr->createPlayer(mCamera);
	mStarted = true;
}

void OgreApplication::exitGame() {
	mStarted = false;
	mDebugOverlay->hide();
	mCamera->setAutoTracking(false); //Supprime une référence sur un noeud qui va disparaître
	mPlayer = NULL; // Par précaution
	delete mObjectMgr; //Nettoie la scène.
}

void OgreApplication::createScene() {
	// Set ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.8, 0.8, 0.8));
//	mSceneMgr->setFog(FOG_EXP2, ColourValue(0.8, 0.8, 0.9));
	mViewPort->setBackgroundColour(ColourValue(0.6,0.6,1.0));

	mSceneMgr->setSkyDome(true, "Wizard/CloudySky", 2, 10);
	// set shadow properties
	mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);//TEXTURE_ADDITIVE);

	// add a bright light above the scene
	Light* light = mSceneMgr->createLight("sun");
	light->setType(Light::LT_DIRECTIONAL);
	light->setDirection(Vector3(10, -40, -20));
	light->setCastShadows(true);

	//Ajout de plein de cubes
	mObjectMgr->loadScene();

}

void OgreApplication::loadResources() {

	ConfigFile configFile;
	configFile.load("config/resources.cfg");
	ConfigFile::SectionIterator seci = configFile.getSectionIterator();
	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
		}
	}
}

void OgreApplication::createCamera() {
    // Create the camera
    mCamera = mSceneMgr->createCamera("MainCamera");
}

void OgreApplication::createViewPort() {
    mViewPort = mWindow->addViewport(mCamera);
    mViewPort->setBackgroundColour(ColourValue(0,0,0));
    mCamera->setAspectRatio(Real(mViewPort->getActualWidth()) / Real(mViewPort->getActualHeight()));
}

void OgreApplication::createFrameListener() {
	startOIS();
	mRoot->addFrameListener(this);
}

bool OgreApplication::frameRenderingQueued(const FrameEvent& evt) {
	if(mWindow->isClosed())
        return false;

    mKeyboard->capture();
    mMouse->capture();

    if (mStarted) {
    	// On met à jour les informations de debug si besoin
    	if (mDebugOverlay->isVisible()) {
    		updateDebugInfo(evt.timeSinceLastFrame);
    	}

    	// Calcul des modifications sur les objets de la scène
    	mObjectMgr->updateObjects(evt.timeSinceLastFrame);
    }

    return mContinue;
}

void OgreApplication::startOIS() {
	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	windowResized(mWindow);
	WindowEventUtilities::addWindowEventListener(mWindow, this);
}

void OgreApplication::windowResized(RenderWindow* rw) {
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void OgreApplication::windowClosed(RenderWindow* rw) {
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}

bool OgreApplication::mouseMoved(const OIS::MouseEvent &e) {
	mMenuMgr.mouseMoved(e);
	if (mStarted && !mLocked)
		mPlayer->injectMouseMove(e);

	return true;
}

bool OgreApplication::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	mMenuMgr.mouseButtonDown(id);
	if (mStarted && !mLocked)
		mPlayer->injectMouseDown(e, id);

	return true;
}

bool OgreApplication::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	mMenuMgr.mouseButtonUp(id);
	return true;
}

bool OgreApplication::keyPressed(const OIS::KeyEvent &e) {
	mMenuMgr.keyPressed(e);
	// Si une partie est en cours
	if (mStarted) {
		switch (e.key) {
		case OIS::KC_ESCAPE:
			mMenuMgr.togglePauseMenu();
			break;
		case OIS::KC_F8: //Toggle bounding boxes
			mSceneMgr->showBoundingBoxes(!mSceneMgr->getShowBoundingBoxes());
			break;
		case OIS::KC_F3: //Toggle debug overlay
			toggleDebugOverlay();
			break;
		default:
			if (!mLocked)
				mPlayer->injectKeyDown(e);
			break;
		}
	}

	return true;
}

bool OgreApplication::keyReleased(const OIS::KeyEvent &e) {
	mMenuMgr.keyReleased(e);

	if (mStarted && !mLocked)
		mPlayer->injectKeyUp(e);

	return true;
}

void OgreApplication::toggleDebugOverlay() {
	if (mDebugOverlay->isVisible())
		mDebugOverlay->hide();
	else
		mDebugOverlay->show();
}

void OgreApplication::updateDebugInfo(Real deltaTime) {
	OverlayContainer* debugPanel = mDebugOverlay->getChild("Wizard/DebugPanel");

	// Mise à jour des FPS
	debugPanel->getChild("Wizard/DebugPanel/Fps")->setCaption(
			"FPS : " + StringConverter::toString(int((double)1.0/(double)deltaTime)));


	// Mise à jour de la position
	Real x = mPlayer->getNode()->getPosition().x,
			y = mPlayer->getNode()->getPosition().y,
			z = mPlayer->getNode()->getPosition().z;
	debugPanel->getChild("Wizard/DebugPanel/Xposition")->setCaption(
			"X : " + StringConverter::toString(x));
	debugPanel->getChild("Wizard/DebugPanel/Yposition")->setCaption(
				"Y : " + StringConverter::toString(y));
	debugPanel->getChild("Wizard/DebugPanel/Zposition")->setCaption(
				"Z : " + StringConverter::toString(z));
}

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
	mInputManager(NULL),
	mMouse(NULL),
	mKeyboard(NULL),
	mObjectMgr(NULL),
	mMenuMgr(this),
	mDebugOverlay(NULL),
	mPlayer(NULL),
	mConfig("config/wizard.cfg"),
	mContinue(true),
	mStarted(false),
	mLocked(true),
	mRequestEndGame(false)
{}

OgreApplication::~OgreApplication() {
#ifdef DEBUG_MODE
LOG("enter OgreApplication destructor");
#endif
    if (mObjectMgr != NULL)
        delete mObjectMgr;
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
#ifdef DEBUG_MODE
LOG("window closed");
#endif
	delete mRoot;
// NE PLUS LOGGER A PARTIR DE MAINTENANT !
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

#ifdef DEBUG_MODE
LOG("stop rendering");
#endif
	return true;
}

void OgreApplication::startGame() {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::startGame");
#endif
	mObjectMgr = new ObjectManager(mSceneMgr);
	createScene();
	mPlayer = mObjectMgr->createPlayer(mCamera);
	mPlayer->addListener(this); // Pour détecter la fin de partie.
	mStarted = true;
#ifdef DEBUG_MODE
LOG("exit OgreApplication::startGame");
#endif
}

void OgreApplication::exitGame() {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::exitGame");
#endif
	mStarted = false;
	mRequestEndGame = false; //normalement déjà à false, mais autant s'en assurer.
	mDebugOverlay->hide();
	mCamera->setAutoTracking(false); //Supprime une référence sur un noeud qui va disparaître
	delete mObjectMgr; //Nettoie la scène.
	mPlayer = NULL; // Par précaution
    mObjectMgr = NULL; // Par précaution

	mMenuMgr.showMainMenu();
#ifdef DEBUG_MODE
LOG("exit OgreApplication::exitGame");
#endif
}

void OgreApplication::createScene() {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::createScene");
#endif
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

	// Chargement de la scène physique
	mObjectMgr->loadScene();
#ifdef DEBUG_MODE
LOG("exit OgreApplication::createScene");
#endif
}

void OgreApplication::loadResources() {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::loadResources");
#endif
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
#ifdef DEBUG_MODE
LOG("exit OgreApplication::loadResources");
#endif
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
#ifdef DEBUG_MODE
LOG("enter OgreApplication::createFrameListener");
#endif
	startOIS();
	mRoot->addFrameListener(this);
#ifdef DEBUG_MODE
LOG("exit OgreApplication::createFrameListener");
#endif
}

#ifdef _WINDOWS
bool OgreApplication::frameStarted(const FrameEvent& evt) {
#else
bool OgreApplication::frameRenderingQueued(const FrameEvent& evt) {
#endif
#ifdef DEBUG_MODE
LOG("enter OgreApplication::frameRenderingQueued");
#endif
	if(mWindow->isClosed())
        return false;

    mKeyboard->capture();
    mMouse->capture();

    if (mRequestEndGame) {
    	mRequestEndGame = false;
    	exitGame();
    }
    else if (mStarted) {
#ifdef DEBUG_MODE
LOG("mStarted = true");
#endif
    	// On met à jour les informations de debug si besoin
    	if (mDebugOverlay->isVisible()) {
    		updateDebugInfo(evt.timeSinceLastFrame);
    	}

    	// Calcul des modifications sur les objets de la scène
    	mObjectMgr->updateObjects(evt.timeSinceLastFrame);
    }
#ifdef DEBUG_MODE
LOG("exit OgreApplication::frameRenderingQueued");
#endif
    return mContinue;
}

/*
 * Initialise les entrees/sorties. Ne pas trop chercher...
 */
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
#ifdef DEBUG_MODE
LOG("enter OgreApplication::mousePressed" + Ogre::StringConverter::toString(id));
#endif
	mMenuMgr.mouseButtonDown(id);
	if (mStarted && !mLocked)
		mPlayer->injectMouseDown(e, id);
#ifdef DEBUG_MODE
LOG("exit OgreApplication::mousePressed");
#endif
	return true;
}

bool OgreApplication::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::mouseReleased "+ Ogre::StringConverter::toString(id));
#endif
	mMenuMgr.mouseButtonUp(id);
#ifdef DEBUG_MODE
LOG("exit OgreApplication::mouseReleased");
#endif
	return true;
}

bool OgreApplication::keyPressed(const OIS::KeyEvent &e) {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::keyPressed");
#endif
    mMenuMgr.keyPressed(e);
	// Si une partie est en cours
	if (mStarted) {
#ifdef DEBUG_MODE
LOG("mStarted = true");
#endif
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
#ifdef DEBUG_MODE
LOG("exit OgreApplication::keyPressed");
#endif
	return true;
}

bool OgreApplication::keyReleased(const OIS::KeyEvent &e) {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::keyReleased");
#endif
    mMenuMgr.keyReleased(e);
	if (mStarted && !mLocked)
		mPlayer->injectKeyUp(e);
#ifdef DEBUG_MODE
LOG("exit OgreApplication::keyReleased");
#endif
	return true;
}

void OgreApplication::toggleDebugOverlay() {
	if (mDebugOverlay->isVisible())
		mDebugOverlay->hide();
	else
		mDebugOverlay->show();
}

void OgreApplication::updateDebugInfo(Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::updateDebugInfo");
#endif
	OverlayContainer* debugPanel = mDebugOverlay->getChild("Wizard/DebugPanel");

	// Mise à jour des FPS
	debugPanel->getChild("Wizard/DebugPanel/Fps")->setCaption(
			"FPS : " + StringConverter::toString(int((double)1.0/(double)deltaTime)));


	// Mise à jour de la position
	Real x = mPlayer->getPosition().x,
			y = mPlayer->getPosition().y,
			z = mPlayer->getPosition().z;
	debugPanel->getChild("Wizard/DebugPanel/Xposition")->setCaption(
			"X : " + StringConverter::toString(x));
	debugPanel->getChild("Wizard/DebugPanel/Yposition")->setCaption(
				"Y : " + StringConverter::toString(y));
	debugPanel->getChild("Wizard/DebugPanel/Zposition")->setCaption(
				"Z : " + StringConverter::toString(z));
#ifdef DEBUG_MODE
LOG("exit OgreApplication::updateDebugInfo");
#endif
}

void OgreApplication::objectDied(const PhysicalObject* object) {
	/* Si cette fonction a été appelée, c'est que le joueur a été tué,
	 * donc GAME OVER.
	 */
	mRequestEndGame = true;
}

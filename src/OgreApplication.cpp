/*
Author: Gecko
*/
#include "OgreApplication.h"

const Ogre::String OgreApplication::MoveForward = "MoveForward";
const Ogre::String OgreApplication::MoveBackward = "MoveBackward";
const Ogre::String OgreApplication::MoveLeft = "MoveLeft";
const Ogre::String OgreApplication::MoveRight = "MoveRight";
const Ogre::String OgreApplication::Jump = "Jump";
const Ogre::String OgreApplication::Action1 = "Action1";
const Ogre::String OgreApplication::Action2 = "Action2";
const Ogre::String OgreApplication::Menu = "Menu";

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
	mConfig.saveConfiguration();
    if (mObjectMgr != NULL)
        delete mObjectMgr;
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
#ifdef DEBUG_MODE
LOG("window closed");
#endif
	delete mRoot;
// NE PLUS LOGGER A PARTIR DE MAINTENANT !
}

Config& OgreApplication::getConfig() {
	return mConfig;
}

bool OgreApplication::start() {
	mRoot = new Ogre::Root("config/plugins.cfg", "config/ogre.cfg", "log/Wizard.log");

	loadResources();

	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	    return false;

	mWindow = mRoot->initialise(true, "Wizard");
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "Wizard Scene Manager");
	mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Wizard/DebugOverlay");
	createCamera();
	createViewPort();
	loadDefaultConfig();
	mMenuMgr.setup();
	createFrameListener();
	mMenuMgr.showMainMenu();
	mRoot->startRendering();

#ifdef DEBUG_MODE
LOG("stop rendering");
#endif
	return true;
}

void OgreApplication::loadDefaultConfig() {
	if (!mConfig.isKeyDefined(MoveForward))
#ifdef _WINDOWS
		mConfig.setValue(MoveForward,OIS::KC_W);
#else
		mConfig.setValue(MoveForward,OIS::KC_Z);
#endif
	if (!mConfig.isKeyDefined(MoveLeft))
#ifdef _WINDOWS
		mConfig.setValue(MoveLeft,OIS::KC_A);
#else
		mConfig.setValue(MoveLeft,OIS::KC_Q);
#endif
	if (!mConfig.isKeyDefined(MoveBackward))
		mConfig.setValue(MoveBackward, OIS::KC_S);

	if (!mConfig.isKeyDefined(MoveRight))
		mConfig.setValue(MoveRight, OIS::KC_D);

	if (!mConfig.isKeyDefined(Jump))
		mConfig.setValue(Jump, OIS::KC_SPACE);

	if (!mConfig.isKeyDefined(Action1))
			mConfig.setValue(Action1, OIS::MB_Left);

	if (!mConfig.isKeyDefined(Action2))
				mConfig.setValue(Action2, OIS::MB_Right);

	if (!mConfig.isKeyDefined(Menu))
				mConfig.setValue(Menu, OIS::KC_ESCAPE);
}

void OgreApplication::startGame() {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::startGame");
#endif
	mObjectMgr = new ObjectManager(mSceneMgr);
	createScene();
	mPlayer = new Player(this, mObjectMgr->createWizard(Ogre::Vector3(1,10,1)), mCamera);
	mPlayer->getCharacter()->addListener(this); // Pour détecter la fin de partie.
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
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));
//	mSceneMgr->setFog(FOG_EXP2, ColourValue(0.8, 0.8, 0.9));
	mViewPort->setBackgroundColour(Ogre::ColourValue(0.6,0.6,1.0));

	mSceneMgr->setSkyDome(true, "Wizard/CloudySky", 2, 10);
	// set shadow properties
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);//TEXTURE_ADDITIVE);

	// add a bright light above the scene
	Ogre::Light* light = mSceneMgr->createLight("sun");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(10, -40, -20));
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
Ogre::ConfigFile configFile;
	configFile.load("config/resources.cfg");
	Ogre::ConfigFile::SectionIterator seci = configFile.getSectionIterator();
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
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
    mViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));
    mCamera->setAspectRatio(Ogre::Real(mViewPort->getActualWidth()) / Ogre::Real(mViewPort->getActualHeight()));
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
bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
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
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
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
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

void OgreApplication::windowResized(Ogre::RenderWindow* rw) {
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void OgreApplication::windowClosed(Ogre::RenderWindow* rw) {
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
		if (e.key == mConfig.getIntValue(Menu))
            mMenuMgr.togglePauseMenu();
		else if (e.key == OIS::KC_F8)
			mSceneMgr->showBoundingBoxes(!mSceneMgr->getShowBoundingBoxes());
		else if (e.key == OIS::KC_F3)
			toggleDebugOverlay();
		else if (!mLocked)
				mPlayer->injectKeyDown(e);
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

void OgreApplication::updateDebugInfo(Ogre::Real deltaTime) {
#ifdef DEBUG_MODE
LOG("enter OgreApplication::updateDebugInfo");
#endif
Ogre::OverlayContainer* debugPanel = mDebugOverlay->getChild("Wizard/DebugPanel");

	// Mise à jour des FPS
	debugPanel->getChild("Wizard/DebugPanel/Fps")->setCaption(
			"FPS : " + Ogre::StringConverter::toString(int((double)1.0/(double)deltaTime)));


	// Mise à jour de la position
	Ogre::Real x = mPlayer->getCharacter()->getPosition().x,
			y = mPlayer->getCharacter()->getPosition().y,
			z = mPlayer->getCharacter()->getPosition().z;
	debugPanel->getChild("Wizard/DebugPanel/Xposition")->setCaption(
			"X : " + Ogre::StringConverter::toString(x));
	debugPanel->getChild("Wizard/DebugPanel/Yposition")->setCaption(
				"Y : " + Ogre::StringConverter::toString(y));
	debugPanel->getChild("Wizard/DebugPanel/Zposition")->setCaption(
				"Z : " + Ogre::StringConverter::toString(z));
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

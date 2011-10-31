/*
Author: Gecko
*/
#include "OgreApplication.h"

OgreApplication::OgreApplication() :
	mRoot(0),
	mWindow(0),
	mSceneMgr(0),
	mCamera(0),
	mViewPort(0),
//	mTrayMgr(0),
	mObjectMgr(0),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0),
	mPlayer(0),
	mContinue(true)
{}

OgreApplication::~OgreApplication() {
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
//	delete mPlayer;
	delete mObjectMgr;
//	delete mTrayMgr;
	delete mRoot;
}

bool OgreApplication::start() {
//	mRoot = Root();
	mRoot = new Root("config/plugins.cfg", "config/ogre.cfg", "log/Wizard.log");

	loadResources();

	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	    return false;

	mWindow = mRoot->initialise(true, "Wizard");
	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager", "Wizard Scene Manager");
	mObjectMgr = new ObjectManager(mSceneMgr);
//	mCollisionsMgr = new MOC::CollisionTools(mSceneMgr);

	createCamera();
	createViewPort();
//	createTrayManager();
	createScene();
	createFrameListener();

	mPlayer = mObjectMgr->createPlayer(mCamera);
	mRoot->startRendering();

	return true;
}

void OgreApplication::createScene() {
	// Set ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.8, 0.8, 0.8));
//	mSceneMgr->setFog(FOG_EXP2, ColourValue(0.8, 0.8, 0.9));
	mViewPort->setBackgroundColour(ColourValue(0.6,0.6,1.0));

	mSceneMgr->setSkyDome(true, "Wizard/CloudySky", 2, 10);
	// set shadow properties
	mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);//TEXTURE_ADDITIVE);

//	// add a bright light above the scene
	Light* light = mSceneMgr->createLight("sun");
	light->setType(Light::LT_DIRECTIONAL);
	light->setDirection(Vector3(10, -40, -20));
	light->setCastShadows(true);

	// Ajoute un sol
	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_Y, 0), 1000, 1000, 100, 100, true, 1, 100, 100, Vector3::UNIT_Z);
	Entity* floor = mSceneMgr->createEntity("Floor", "floor");
	floor->setMaterialName("Wizard/GrassFloor");
	floor->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeFloor")->attachObject(floor);

	//Ajout de plein de cubes
//	mObjectMgr->loadScene();

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
    mCamera = mSceneMgr->createCamera("PlayerCam");
//    mCamera->setPosition(Vector3(0,0,20));
//    mCamera->lookAt(Vector3(0,0,0));
//    mCamera->setNearClipDistance(0.1);
//    mCamera->setFarClipDistance(20);
}

void OgreApplication::createViewPort() {
    mViewPort = mWindow->addViewport(mCamera);
    mViewPort->setBackgroundColour(ColourValue(0,0,0));
    mCamera->setAspectRatio(Real(mViewPort->getActualWidth()) / Real(mViewPort->getActualHeight()));
}

//void OgreApplication::createTrayManager() {
//	mTrayMgr = new OgreBites::SdkTrayManager("WizardTrayManager", mWindow, mMouse);
//}

void OgreApplication::createFrameListener() {
	startOIS();
	mRoot->addFrameListener(this);
}

bool OgreApplication::frameRenderingQueued(const FrameEvent& evt) {
    if(mWindow->isClosed())
        return false;

    mKeyboard->capture();
    mMouse->capture();

//    mPlayer->addTime(evt.timeSinceLastFrame);
    mObjectMgr->updateObjects(evt.timeSinceLastFrame);

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
	mPlayer->injectMouseMove(e);
	return true;
}

bool OgreApplication::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	mPlayer->injectMouseDown(e, id);
	return true;
}

bool OgreApplication::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	return true;
}

bool OgreApplication::keyPressed(const OIS::KeyEvent &e) {
	switch (e.key) {
	case OIS::KC_ESCAPE:
		mContinue = false;
		break;
	case OIS::KC_F8: //Toggle bounding boxes
		mSceneMgr->showBoundingBoxes(!mSceneMgr->getShowBoundingBoxes());
	default:
		mPlayer->injectKeyDown(e);
	}

	return true;
}

bool OgreApplication::keyReleased(const OIS::KeyEvent &e) {
	mPlayer->injectKeyUp(e);

	return true;
}

/*
Author: Gecko
*/
#include "OgreApplication.h"
#include "objects/Player.h"

// Méthode statique pour convertir les évènements de souris de OIS à CEGUI
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) {
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

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
	mDebugOverlay(NULL),
	mCeguiRenderer(NULL),
	mMenuSheet(NULL),
	mPlayer(NULL),
	mContinue(true),
	mLocked(false)
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
	mObjectMgr = new ObjectManager(mSceneMgr);
	mDebugOverlay = OverlayManager::getSingleton().getByName("Wizard/DebugOverlay");

	createCamera();
	createViewPort();
	createScene();
	setupGUI();
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

	// add a bright light above the scene
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
    mCamera = mSceneMgr->createCamera("PlayerCam");
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

    // On met à jour les informations de debug si besoin
    if (mDebugOverlay->isVisible()) {
    	updateDebugInfo(evt.timeSinceLastFrame);
    	}

    // Calcul des modifications sur les objets de la scène
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
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(e.state.X.rel, e.state.Y.rel);
	// Scroll wheel.
	if (e.state.Z.rel)
	    sys.injectMouseWheelChange(e.state.Z.rel / 120.0f); //120 = nombre magique à ne pas changer.

	if (!mLocked)
		mPlayer->injectMouseMove(e);

	return true;
}

bool OgreApplication::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	if (!mLocked)
		mPlayer->injectMouseDown(e, id);

	return true;
}

bool OgreApplication::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

bool OgreApplication::keyPressed(const OIS::KeyEvent &e) {
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(e.key);
	sys.injectChar(e.text);

	switch (e.key) {
	case OIS::KC_ESCAPE:
		toggleMenu();
		break;
	case OIS::KC_F4:
		mContinue = false;
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

	return true;
}

bool OgreApplication::keyReleased(const OIS::KeyEvent &e) {
	CEGUI::System::getSingleton().injectKeyUp(e.key);

	if (!mLocked)
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

void OgreApplication::setupGUI() {
	mCeguiRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	//Test
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	mMenuSheet = wmgr.createWindow("DefaultWindow", "WizardMenu/Sheet");
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "WizardMenu/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.08, 0)));
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
	mMenuSheet->addChildWindow(quit);
	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreApplication::quit, this));
}

void OgreApplication::toggleMenu() {
	if (CEGUI::System::getSingleton().getGUISheet() == mMenuSheet) {
//		CEGUI::System::getSingleton().setGUISheet(NULL);
		mLocked = false;
	}
	else {
		mLocked = true;
		CEGUI::System::getSingleton().setGUISheet(mMenuSheet);
	}
}

bool OgreApplication::quit(const CEGUI::EventArgs &e) {
	mContinue = false;
	return true;
}

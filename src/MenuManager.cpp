/*
 * MenuManager.cpp
 *
 *  Created on: 6 nov. 2011
 *      Author: gecko
 */

#include "MenuManager.h"

#include "OgreApplication.h"

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

MenuManager::MenuManager(OgreApplication* app) :
	mApp(app),
	mSys(NULL),
	mCeguiRenderer(NULL),
	mMainSheet(NULL),
	mPauseSheet(NULL),
	mBlankSheet(NULL),
	mSettingsSheet(NULL),
	mLastSheet(NULL),
	mDarkOverlay(NULL)
{}

MenuManager::~MenuManager() {}

bool MenuManager::mouseMoved(const OIS::MouseEvent &e) {
	mSys->injectMouseMove(e.state.X.rel, e.state.Y.rel);
	// Scroll wheel.
	if (e.state.Z.rel)
		mSys->injectMouseWheelChange(e.state.Z.rel / 120.0f); //120 = nombre magique à ne pas changer.
	return true;
}

bool MenuManager::mouseButtonDown(OIS::MouseButtonID id) {
	mSys->injectMouseButtonDown(convertButton(id));
	return true;
}

bool MenuManager::mouseButtonUp(OIS::MouseButtonID id) {
	mSys->injectMouseButtonUp(convertButton(id));
	return true;
}

bool MenuManager::keyPressed(const OIS::KeyEvent &e) {
	mSys->injectKeyDown(e.key);
	mSys->injectChar(e.text);
	return true;
}

bool MenuManager::keyReleased(const OIS::KeyEvent &e) {
	CEGUI::System::getSingleton().injectKeyUp(e.key);
	return true;
}

void MenuManager::setup() {
	mDarkOverlay = Ogre::OverlayManager::getSingleton().getByName("Wizard/DarkOverlay");
#ifdef _WINDOWS
    mCeguiRenderer = new CEGUI::OgreCEGUIRenderer(mApp->mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mApp->mSceneMgr);
    mSys = new CEGUI::System(mCeguiRenderer);
#else
	mCeguiRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	mSys = CEGUI::System::getSingletonPtr();
#endif
	CEGUI::Imageset::setDefaultResourceGroup("GUI");
	CEGUI::Font::setDefaultResourceGroup("GUI");
	CEGUI::Scheme::setDefaultResourceGroup("GUI");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("GUI");
	CEGUI::WindowManager::setDefaultResourceGroup("GUI");
#ifdef _WINDOWS
	CEGUI::SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
#else
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
#endif
	mSys->setDefaultMouseCursor("TaharezLook", "MouseArrow");
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	//Configuration d'une GUI vierge (utilisée pendant le jeu)
	mBlankSheet = wmgr.loadWindowLayout("GUI/Blank.layout");

	// Configuration du menu principal
	mMainSheet = wmgr.loadWindowLayout("GUI/Main.layout");
	mMainSheet->getChild("Main/Play")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&MenuManager::startGame, this));
	mMainSheet->getChild("Main/Quit")->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&MenuManager::quit, this));
                                                                                                        Ogre::LogManager::getSingleton().logMessage("5");
	// Configuration du menu Pause
	mPauseSheet = wmgr.loadWindowLayout("GUI/Pause.layout");
	mPauseSheet->getChild("Pause/Back")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&MenuManager::back, this));
	mPauseSheet->getChild("Pause/Settings")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&MenuManager::displaySettings, this));
	mPauseSheet->getChild("Pause/Quit")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&MenuManager::exitGame, this));
                                                                                                        Ogre::LogManager::getSingleton().logMessage("6");
	// Configuration du menu Options
	mSettingsSheet = wmgr.loadWindowLayout("GUI/Settings.layout");
	mSettingsSheet->getChild("Settings/Shadows")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&MenuManager::configureShadows, this));
	mSettingsSheet->getChild("Settings/Back")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&MenuManager::back, this));
}

void MenuManager::togglePauseMenu() {
	if (mSys->getGUISheet() == mPauseSheet)
		hideMenus();
	else {
		mApp->mLocked = true;
		mSys->setGUISheet(mPauseSheet);
		mDarkOverlay->show();
		CEGUI::MouseCursor::getSingleton().show();
	}
}

void MenuManager::showMainMenu() {
	mApp->mLocked = true;
	mSys->setGUISheet(mMainSheet);
	CEGUI::MouseCursor::getSingleton().show();
}

void MenuManager::hideMenus() {
	mApp->mLocked = false;
	mSys->setGUISheet(mBlankSheet);
	mDarkOverlay->hide();
	CEGUI::MouseCursor::getSingleton().hide();
}

bool MenuManager::quit(const CEGUI::EventArgs &e) {
	mApp->mContinue = false;
	return true;
}

bool MenuManager::back(const CEGUI::EventArgs &e) {
	// Si on vient d'un autre menu
	if (mLastSheet) {
		mSys->setGUISheet(mLastSheet);
		mLastSheet = NULL;
	}
	else
		togglePauseMenu();
	return true;
}

bool MenuManager::startGame(const CEGUI::EventArgs &e) {
	mApp->startGame();
	hideMenus();
	return true;
}

bool MenuManager::exitGame(const CEGUI::EventArgs &e) {
	mApp->exitGame();
	return true;
}

bool MenuManager::displaySettings(const CEGUI::EventArgs &e) {
	mLastSheet = mPauseSheet;
	mSys->setGUISheet(mSettingsSheet);
	return true;
}

bool MenuManager::configureShadows(const CEGUI::EventArgs &e) {
	CEGUI::Window* button = mSettingsSheet->getChild("Settings/Shadows");

	switch (mApp->mSceneMgr->getShadowTechnique()) {
	case SHADOWTYPE_NONE:
		button->setText("Ombres : Faibles");
		mApp->mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);
		break;
	case SHADOWTYPE_TEXTURE_ADDITIVE:
		button->setText((CEGUI::utf8*)"Ombres : Précises");
		mApp->mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
		break;
	default:
		button->setText("Ombres : Non");
		mApp->mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);
		break;
	}
	return true;
}

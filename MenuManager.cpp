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
	mPauseSheet(NULL),
	mBlankSheet(NULL)
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
	mCeguiRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	mSys = CEGUI::System::getSingletonPtr();
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	mSys->setDefaultMouseCursor("TaharezLook", "MouseArrow");

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	//Configuration d'une GUI vierge
	mBlankSheet = wmgr.loadWindowLayout("GUI/Blank.layout");

	// Configuration du menu Pause
	mPauseSheet = wmgr.loadWindowLayout("GUI/Pause.layout");
	mPauseSheet->getChild("Pause/Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuManager::quit, this));
}

void MenuManager::toggleMenu() {
	if (mSys->getGUISheet() == mPauseSheet) {
		mApp->mLocked = false;
		mSys->setGUISheet(mBlankSheet);
		CEGUI::MouseCursor::getSingleton().hide();
	}
	else {
		mApp->mLocked = true;
		mSys->setGUISheet(mPauseSheet);
		CEGUI::MouseCursor::getSingleton().show();
	}
}

bool MenuManager::quit(const CEGUI::EventArgs &e) {
	mApp->mContinue = false;
	return true;
}


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
	mCeguiRenderer(NULL),
	mPauseSheet(NULL)
{}

MenuManager::~MenuManager() {}

bool MenuManager::mouseMoved(const OIS::MouseEvent &e) {
	CEGUI::System& sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(e.state.X.rel, e.state.Y.rel);
	// Scroll wheel.
	if (e.state.Z.rel)
		sys.injectMouseWheelChange(e.state.Z.rel / 120.0f); //120 = nombre magique à ne pas changer.
	return true;
}

bool MenuManager::mouseButtonDown(OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}

bool MenuManager::mouseButtonUp(OIS::MouseButtonID id) {
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

bool MenuManager::keyPressed(const OIS::KeyEvent &e) {
	CEGUI::System& sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(e.key);
	sys.injectChar(e.text);
	return true;
}

bool MenuManager::keyReleased(const OIS::KeyEvent &e) {
	CEGUI::System::getSingleton().injectKeyUp(e.key);
	return true;
}

void MenuManager::setup() {
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
	mPauseSheet = wmgr.createWindow("DefaultWindow", "WizardMenu/Sheet");
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "WizardMenu/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.08, 0)));
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
	mPauseSheet->addChildWindow(quit);
	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuManager::quit, this));
}

void MenuManager::toggleMenu() {
	if (CEGUI::System::getSingleton().getGUISheet() == mPauseSheet) {
		mApp->mLocked = false;
	}
	else {
		mApp->mLocked = true;
		CEGUI::System::getSingleton().setGUISheet(mPauseSheet);
	}
}

bool MenuManager::quit(const CEGUI::EventArgs &e) {
	mApp->mContinue = false;
	return true;
}


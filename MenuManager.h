/*
 * MenuManager.h
 *
 *  Created on: 6 nov. 2011
 *      Author: gecko
 */

#ifndef MENUMANAGER_H_
#define MENUMANAGER_H_

#include "CEGUI.h"
#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#include <OIS.h>

//forward declaration
class OgreApplication;

/**
 * Classe étroitement liée à OgreApplication.
 * Elle sert à gérer séparément ce qui concerne les menus.
 */
class MenuManager {
public:
	MenuManager(OgreApplication* app);
	virtual ~MenuManager();
	/**
	 * Méthode qui initialise la GUI.
	 * A n'appeler qu'une fois que Ogre est initialisé, sinon CEGUIOgreRenderer plante.
	 */
	void setup();
	bool mouseMoved(const OIS::MouseEvent &e);
	bool mouseButtonDown(OIS::MouseButtonID id);
	bool mouseButtonUp(OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);
	void togglePauseMenu();

private:
	/**
	 * Quitte le jeu.
	 */
	bool quit(const CEGUI::EventArgs &e);
	/**
	 * Retour au menu précédent.
	 */
	bool back(const CEGUI::EventArgs &e);
	/**
	 * Affiche le menu des options
	 */
	bool displaySettings(const CEGUI::EventArgs &e);
	/**
	 * Change le réglage des ombres
	 */
	bool configureShadows(const CEGUI::EventArgs &e);

	OgreApplication* mApp;
	CEGUI::System* mSys;
	CEGUI::OgreRenderer* mCeguiRenderer;
	CEGUI::Window* mPauseSheet;
	CEGUI::Window* mBlankSheet;
	CEGUI::Window* mSettingsSheet;
	CEGUI::Window* mLastSheet;

};

#endif /* MENUMANAGER_H_ */

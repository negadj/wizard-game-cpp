# Introduction #

Cette page décrit la marche à suivre pour configurer son environnement de développement pour ce projet. La plupart des consignes concernant Windows proviennent de cette [page](http://www.ogre3d.org/tikiwiki//Codeblocks+and+MinGW).

# Installation #
## Démarche globale ##

Pour développer sur ce projet, il faut :
  * Un IDE (Eclipse ou Code::Blocks)
  * Mercurial (comme gestionnaire de version)
  * le SDK Ogre
  * la bibliothèque CEGUI

## Installation sous Linux ##
Pour Linux, Eclipse est très pratique pour gérer ce projet, en particulier grâce à son plugin Mercurial. Si vous voulez, vous pouvez bien entendu utiliser un autre IDE comme Code::Blocks.

Pour archlinux :
```
yaourt -Sy eclipse-cdt eclipse-mercurial ogre cegui
```

Pour les distributions n'ayant pas de paquets pour CDT et/ou le plugin Mercurial :

### Installation de CDT ###
Depuis Eclipse :  Help->Install New Software puis sélectionner le dépot de votre version d'Eclipse (indigo en ce moment), et dans Programming langages, sélectionner C/C++ Development Tools (CDT).

### Installation de Mercurial ###
Idem, mais cette fois l'adresse du dépot est http://cbes.javaforge.com/update.
Installer MercurialEclipse.

## Installation sous Windows ##
Pour Windows en revanche, il est recommandé de suivre cette démarche et d'utiliser Code::Blocks. Un tuto pour faire marcher le projet sous Eclipse serait le bienvenu.

### Installation du Kit Windows ###
La solution trouvée actuellement pour avoir un environnement qui marche pour Windows repose sur des verions du SdK qui datent un peu. Il est recommandé d'utiliser le kit fourni plutôt que d'essayer tout seul. Cependant, toute méthode d'installation pour faire fonctionner un SDK récent d'ogre avec autre chose que Visual Studio est la bienvenue.

Téléchargez  le [Kit](http://wizard-game-cpp.googlecode.com/files/Wizard_Win32Kit.zip) contenant les installateurs. Il faut installer MinGW impérativement avant Code::Blocks.
Installer les trois programmes.


### Installation de Mercurial ###
Nous recommandons l'utilisation de TortoiseHG, à défaut d'avoir u plugin Mercurial pour Code::Blocks (les utilisateurs d'Eclipse peuvent se réfèrer à l'instalaltion de EclipseMercurial).

# Configuration avec Eclipse (Linux) #

## Création du projet ##
Dans Eclipse, créer un nouveau projet. Choisir "C++ project" et lui donner un nom (Wizard).

## Options du compilateur ##
Ouvrir les propriétés du projet et dans C/C++ Build->Settings, choisir dans la fenêtre de droite les options du compilateur C++ (GCC C++ Compiler ->Miscellaneous) et s'assurer que ces options sont présentes :
```
-fmessage-length=0 -mthreads -fexceptions -fident
```

Si vous êtes sous Windows, dans les symboles de préprocesseur, ajoutez les deux symboles (pour toutes les configurations) :
```
WIN32
_WINDOWS
```
Il faut maintenant déclarer nos bibliothèques. Cliquez sur le sous-menu "Includes", et ajoutez dans les include paths les dossiers suivants (attention aux \ ou / selon que vous soyez sous Windows ou Linux) :
```
C:\OgreSDK\include\OGRE
C:\OgreSDK\include\OIS
C:\OgreSDK\boost_1_44
```

## Options du Linker ##
Sélectionner C++ Linker et rajouter dans les options ("other flags" dans Miscellaneous) :
```
-Wl,--enable-auto-image-base
-Wl,--add-stdcall-alias
-Wl,--enable-auto-import
```

Il est possible pour la suite de lier des bibliothèques différentes selon que l'on est en Debug ou Release. On présentera ici la version Release, qu'on appliquera à toute les configurations. TODO: détailler la configuration Debug.

Sélectionner "Libraries" et ajouter les librairies
```
OgreMain
OIS
CEGUIBase
CEGUIOgreRenderer
```

Pour Windows, ne pas oublier de rajouter dans les libraries search path :
```
C:\OgreSDK\bin\Release
```

# Configuration avec Code::Blocks (Windows) #
Créer un nouveau projet, et sélectionnez le modèle Ogre s'il est bien installé sur votre ordinateur. Indiquez que vous avez un pre-build SDK d'Ogre. Normalement, tout est déjà bien réglé :)

# Récupération du projet #
/*
 * Config.cpp
 *
 *  Created on: 20 déc. 2011
 *      Author: gecko
 */

#include "Config.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <Ogre.h>

Config::Config(std::string filename) :
	mFilename(filename),
	mStrValues(std::map<std::string, std::string>()),
	mIntValues(std::map<std::string, int>())
{
	parseConfigFile();
}

Config::~Config() {}

std::string Config::getStringValue(std::string key) {
	return mStrValues[key];
}

int Config::getIntValue(std::string key) {
	return mIntValues[key];
}

void Config::setValue(std::string key, std::string value) {
	mStrValues[key] = value;
	if (Ogre::StringConverter::isNumber(value))
		mIntValues[key] = Ogre::StringConverter::parseInt(value);
}

void Config::setValue(std::string key, int value) {
	mIntValues[key] = value;
	mStrValues[key] = Ogre::StringConverter::toString(value);
}

bool Config::isKeyDefined(std::string key) {
	return (mStrValues.find(key) != mStrValues.end());
}

void Config::parseConfigFile() {
	std::ifstream file(mFilename.c_str());
	if (file) { // fichier correctement ouvert
		std::string line, key, value;
		size_t pos;
		while (std::getline(file, line)) { // On parcourt toute les lignes;
			key = "";
			value = "";
			if (line.at(0) != '#') { // Pour les éventuels commentaires.
				pos = line.find('=');
				if (pos != std::string::npos && pos != 0 && pos != line.size()-1) { // Format a priori correct
					key = line.substr(0,pos);
					value = line.substr(pos+1);
					Ogre::StringUtil::trim(key); // On enlève les éventuels espaces;
					Ogre::StringUtil::trim(value);

					// On stocke la nouvelle paire
					mStrValues[key] = value;
					if (Ogre::StringConverter::isNumber(value))
						mIntValues[key] = Ogre::StringConverter::parseInt(value);
				}
			}
		}
		file.close();
	}
		//TODO: else ?
}

void Config::saveConfiguration() {
	std::ofstream file(mFilename.c_str(), std::ios::out | std::ios::trunc); // fichier écrasé
	        if(file) // fichier correctement ouvert
	        {
	        	for (std::map<std::string, std::string>::iterator it = mStrValues.begin(); it != mStrValues.end(); ++it) {
	        		file << it->first << "=" << it->second << std::endl;
	        	}
	        	file.close();
	        }
	        //TODO: else ?
}

std::string& Config::operator[](std::string key) {
	return mStrValues[key];
}

//int& Config::operator[](std::string key) {
//	return mIntValues[key];
//}

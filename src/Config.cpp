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

void trim(std::string& str) {
	str = str.substr(str.find_first_not_of(' '));
	str = str.substr(0, str.find_last_not_of(' ')+1);
}

Config::Config(std::string filename) :
	mFilename(filename),
	mValues(std::map<std::string, std::string>())
{
	parseConfigFile();
}

Config::~Config() {}

std::string Config::getValue(std::string key) {
	return mValues[key];
}

void Config::setValue(std::string key, std::string value) {
	mValues[key] = value;
}

bool Config::isKeyDefined(std::string key) {
	return (mValues.find(key) != mValues.end());
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
					trim(key); // On enlève les éventuels espaces;
					trim(value);

					// On stocke la nouvelle paire
					mValues[key] = value;
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
	        	for (std::map<std::string, std::string>::iterator it = mValues.begin(); it != mValues.end(); ++it) {
	        		file << it->first << "=" << it->second << std::endl;
	        	}
	        	file.close();
	        }
	        //TODO: else ?
}

std::string& Config::operator[](std::string key) {
	return mValues[key];
}

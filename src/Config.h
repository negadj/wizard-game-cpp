/*
 * Config.h
 *
 *  Created on: 20 déc. 2011
 *      Author: gecko
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <map>

class Config {
private:
	const std::string mFilename;
	std::map<std::string,std::string> mValues;

	void parseConfigFile();

public:
	Config(std::string filename);
	virtual ~Config();
	std::string getValue(std::string key);
	/*
	 * Définit la valeur d'une clé, éventuellement nouvelle.
	 * Attention, cette nouvelle clé/valeur n'est pas sauvegardée par défaut.
	 */
	void setValue(std::string key, std::string value);
	bool isKeyDefined(std::string key);
	void saveConfiguration();
	/*
	 * Comme pour setValue, une affectation ne modifie que la valeur en mémoire.
	 * Pour sauvegarder la configuration, utiliser saveConfiguration();
	 */
	std::string& operator[](std::string key);
};

#endif /* CONFIG_H_ */

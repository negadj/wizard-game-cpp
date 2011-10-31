/*
 * PhysicalObject.h
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include "Ogre.h"

// Pour éviter l'oeuf et la poule
class ObjectManager;

typedef unsigned char ObjectId_t;

class PhysicalObject {
	friend class ObjectManager;
private:
	//TODO: Matériaux ?
	ObjectId_t mId;
	Ogre::String mName;
	Ogre::String mDescription;
	/* contient les informations de position, dimensions, rotation... */
	Ogre::SceneNode* mNode;
	Ogre::Entity* mEntity;
	Ogre::Vector3 mAcceleration;
	Ogre::Vector3 mSpeed;
	int mSolidity;
	float mDensity;
	int mIntegrity; //~= points de vie pour un objet

protected:
	PhysicalObject(Ogre::SceneNode* node, Ogre::String name, int id, Ogre::String meshName, std::string description = "Objet");
	virtual ~PhysicalObject();
	/* Met à jour tout ce qui concerne l'objet hormis les déplacements (qui sont gérés par l'ObjectManager)*/
	virtual void update(Ogre::Real deltaTime);
	void setEntity(Ogre::Entity *mEntity);

public:
	Ogre::Vector3 getAcceleration() const;
    ObjectId_t getId() const;
    int getIntegrity() const;
    std::string getName() const;
    Ogre::SceneNode* getNode() const;
    Ogre::Vector3 getSpeed() const;
    void setAcceleration(Ogre::Vector3 mAcceleration);
    void setId(ObjectId_t mId);
    void setIntegrity(int mIntegrity);
    void setName(std::string mName);
    void setSpeed(Ogre::Vector3 mSpeed);
    float getDensity() const;
    int getSolidity() const;
    void setDensity(float mDensity);
    void setSolidity(int mSolidity);
    Ogre::Entity *getEntity() const;
    Ogre::String getDescription() const;
    void setDescription(Ogre::String mDescription);

};

#endif /* PHYSICALOBJECT_H_ */

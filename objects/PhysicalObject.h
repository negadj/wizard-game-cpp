/*
 * PhysicalObject.h
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include "Ogre.h"

// forward declaration
class ObjectManager;

typedef unsigned char ObjectId_t;

class PhysicalObject {
	friend class ObjectManager;
private:
	//TODO: Matériaux ?
	ObjectManager* mObjectManager;
	ObjectId_t mId;
	const Ogre::String mName;
	Ogre::String mDescription;
	/* contient les informations de position, dimensions, rotation... */
	Ogre::SceneNode* mNode;
	Ogre::Entity* mEntity;
	Ogre::Vector3 mAcceleration;
	Ogre::Vector3 mSpeed;
	int mSolidity;
	float mDensity;
	int mIntegrity; //~= points de vie pour un objet
	Ogre::Vector3 mVolume;

	void setupVolume(Ogre::Vector3 centreMin,Ogre::Vector3 CentreMax);

protected:
	/*
	 * Attention : name doit être unique.
	 */
	PhysicalObject(ObjectManager* objectManager, Ogre::SceneNode* node, Ogre::String name, int id, Ogre::String meshName, Ogre::Vector3 volume, std::string description = "Objet");
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
    void setSpeed(Ogre::Vector3 mSpeed);
    void addSpeed(Ogre::Vector3 speed);
    float getDensity() const;
    int getSolidity() const;
    void setDensity(float mDensity);
    void setSolidity(int mSolidity);
    Ogre::Entity *getEntity() const;
    Ogre::String getDescription() const;
    void setDescription(Ogre::String mDescription);
    ObjectManager* getObjectManager() const;
    const Ogre::Vector3 getVolume() const;
    std::vector<Ogre::Vector3> getContactSurface(const Ogre::Vector3 normal) const;

};

#endif /* PHYSICALOBJECT_H_ */

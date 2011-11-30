/*
 * PhysicalObject.h
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include "Ogre.h"
#include "PhysicalObjectListener.h"

// forward declaration
class ObjectManager;

typedef unsigned char ObjectId_t;
/*
 * Represente un objet physique, pouvant aller d'une fleur à un bloc de granite
 */
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
	Ogre::Vector3 mCollisionCorrection;
	std::set<PhysicalObjectListener*> mListeners;

	void setupVolume(Ogre::Vector3 centreMin,Ogre::Vector3 CentreMax);
	void fireDestruction() const;

protected:
	/*
	 * Attention : name doit être unique. Mieux vaut laisser l'ObjectManager s'en charger seul.
	 */
	PhysicalObject(ObjectManager* objectManager, Ogre::SceneNode* node, Ogre::String name, int id, Ogre::String meshName, Ogre::Vector3 volume, std::string description = "Objet");
	virtual ~PhysicalObject();
	void preCollisionUpdate(Ogre::Real deltaTime);
	void postCollisionUpdate(Ogre::Real deltaTime);
	/* Met à jour tout ce qui concerne l'objet hormis les déplacements (qui sont gérés par l'ObjectManager)*/
	virtual void doPreCollisionUpdate(Ogre::Real deltaTime);
	virtual void doPostCollisionUpdate(Ogre::Real deltaTime);
	void setEntity(Ogre::Entity *mEntity);
	/*
	 * Actions à mener lorsque l'objet est détruit
	 * (i.e. lorsque son intégrité vaut 0, pas lorsque le destructeur est appelé)
	 */
	virtual void destroy();

public:
	Ogre::Vector3 getAcceleration() const;
    ObjectId_t getId() const;
    int getIntegrity() const;
    std::string getName() const;
    Ogre::SceneNode* getNode() const;
    Ogre::Vector3 getSpeed() const;
    void setAcceleration(Ogre::Vector3 acceleration);
    void setId(ObjectId_t mId);
    void setIntegrity(int mIntegrity);

    void setSpeed(Ogre::Vector3 speed);
    void addSpeed(Ogre::Vector3 speed);
    void addForce(Ogre::Vector3 force);

    float getDensity() const;
    int getSolidity() const;
    void setDensity(float density);
    void setSolidity(int solidity);
    Ogre::Entity *getEntity() const;
    Ogre::String getDescription() const;
    void setDescription(Ogre::String description);
    ObjectManager* getObjectManager() const;
    const Ogre::Vector3 getVolume() const;
    std::vector<Ogre::Vector3> getContactSurface(const Ogre::Vector3 normal) const;
    Ogre::Vector3 getCollisionCorrection() const;
    void setCollisionCorrection(Ogre::Vector3 correction);
    bool isOnGround() const;

    void addListener(PhysicalObjectListener* listener);
    void removeListener(PhysicalObjectListener* listener);
};

#endif /* PHYSICALOBJECT_H_ */

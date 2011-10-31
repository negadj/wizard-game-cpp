/*
 * PhysicalObject.h
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#ifndef PHYSICALOBJECT_H_
#define PHYSICALOBJECT_H_

#include "Ogre.h"
//#include "ObjectManager.h"

// Pour éviter les inclusions circulaires
class ObjectManager;

typedef unsigned char ObjectId_t;

class PhysicalObject {
	friend class ObjectManager;
private:
	//	static unsigned int _instanceNumber = 0;
	//TODO: Matériaux ?
	ObjectId_t mId;
	Ogre::String mName;
	/* contient les informations de position, dimensions, rotation... */
	Ogre::SceneNode* mNode;
	Ogre::Vector3 mAcceleration;
	Ogre::Vector3 mSpeed;
//	Ogre::Vector3 mDimensions;
	int mSolidity;
	float mDensity;
	int mIntegrity; //~= points de vie pour un objet

	virtual void setup() = 0;

protected:
	PhysicalObject(Ogre::SceneNode* originNode, int id = 0, std::string name = "Objet");
	/* Met à jour tout ce qui concerne l'objet hormis les déplacements (qui sont gérés par l'ObjectManager)*/
	virtual void update(Ogre::Real deltaTime);

public:
	virtual ~PhysicalObject();
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
//    void setNode(Ogre::SceneNode* mNode);
    void setSpeed(Ogre::Vector3 mSpeed);
    float getDensity() const;
    int getSolidity() const;
    void setDensity(float mDensity);
    void setSolidity(int mSolidity);

};

#endif /* PHYSICALOBJECT_H_ */

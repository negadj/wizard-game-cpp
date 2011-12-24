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
#include "PhysicalMaterial.h"

// forward declaration
class ObjectManager;

typedef enum ObjectType {
	TYPE_FRIENDLY,
	TYPE_HOSTILE,
	TYPE_ANIMATED,
	TYPE_BLOCK,
	TYPE_OBJECT
} ObjectType;


/*
 * Represente un objet physique, pouvant aller d'une fleur à un bloc de granite
 */
class PhysicalObject {
	friend class ObjectManager;
private:
	static unsigned long _countObject; // Sert à attribuer un numéro unique à chaque objet.
	static ObjectManager* _mObjectManager;

	/*
	 * mName fait office d'id unique pour chaque objet.
	 */
	const Ogre::String mName;
	std::set<ObjectType> mTypes;
	const Ogre::String mOriginalMeshName;
	Ogre::String mDescription;
	/* contient les informations de position, dimensions, rotation... */
	Ogre::SceneNode* mNode;
	Ogre::Entity* mEntity;
	Ogre::Vector3 mAcceleration;
	Ogre::Vector3 mSpeed;
	PhysicalMaterial mOriginalMaterial;
	PhysicalMaterial mMaterial;
	int mIntegrity; //~= points de vie pour un objet
	Ogre::Vector3 mVolume;
	Ogre::Vector3 mCollisionCorrection;
	std::set<PhysicalObjectListener*> mListeners;

	/*
	 * Renvoie un nom unique pour définir un objet
	 */
	static Ogre::String _getUniqueName();
	void setupVolume(Ogre::Vector3 centreMin,Ogre::Vector3 CentreMax);

protected:
	PhysicalObject(Ogre::SceneNode* node, Ogre::String meshName, Ogre::Vector3 volume, PhysicalMaterial material, std::string description = "Objet");
	virtual ~PhysicalObject();
	void preCollisionUpdate(Ogre::Real deltaTime);
	void postCollisionUpdate(Ogre::Real deltaTime);
	/* Met à jour tout ce qui concerne l'objet hormis les déplacements (qui sont gérés par l'ObjectManager)*/
	virtual void doPreCollisionUpdate(Ogre::Real deltaTime);
	virtual void doPostCollisionUpdate(Ogre::Real deltaTime);
	void setEntity(Ogre::Entity *entity);
	void registerType(ObjectType type);
	virtual Ogre::SceneNode* getNode() const;
	/*
	 * Actions à mener lorsque l'intégrité d'un objet est modifiée.
	 */
    virtual void onIntegrityChange(int oldIntegrity);
    /*
	 * Actions à mener lorsque l'objet est détruit, "tué"
	 * (i.e. lorsque son intégrité vaut 0, pas lorsque le destructeur est appelé)
	 */
	virtual void die();
	void requestDestruction();
	void fireDeath() const;
	void fireApparenceChanged() const;

//	/*
//	 * Par défaut, l'ObjectManager appelle le destructeur de l'objet lorsque celui-ci
//	 * "meurt" (et qu'il envoie son fireDestruction).
//	 * On peut concevoir que pour certains objets, "mourir" ne signifie pas être retiré de la scène,
//	 * ou tout simplement qu'ils sont "indestructibles".
//	 * Dans ce cas, il faut mettre ce booléen à "false"
//	 */
//	bool mRemoveOnDestroy;

public:
	/*
	 * Quelques directions locales commodes
	 * (pour ne plus avoir à se torturer avec l'orientation des repères)
	 */
	static const Ogre::Vector3 LocalDirectionForward;
	static const Ogre::Vector3 LocalDirectionBackward;
	static const Ogre::Vector3 LocalDirectionLeft;
	static const Ogre::Vector3 LocalDirectionRight;
	static const Ogre::Vector3 LocalDirectionUp;
	static const Ogre::Vector3 LocalDirectionDown;

	static ObjectManager* getObjectManager();

	Ogre::Vector3 getAcceleration() const;
    bool isType(ObjectType type) const;
    int getIntegrity() const;
    std::string getName() const;
    Ogre::Vector3 getSpeed() const;
    /*
     * Indique si l'objet est "actif", i.e. si l'ObjectManager doit le gérer
     * dans sa boucle principale ou non.
     */
    void setActive(bool active);
    bool isActive();
    void setAcceleration(Ogre::Vector3 acceleration);
    void setIntegrity(int mIntegrity);
    const Ogre::Vector3& getPosition() const;
    const Ogre::Vector3 getFacingDirection() const;
    void faceDirection(const Ogre::Vector3& direction);
    void faceLocalDirection(const Ogre::Vector3& direction);
    const Ogre::Quaternion& getOrientation() const;
    void setPosition(const Ogre::Vector3& pos);
    void setOrientation(const Ogre::Quaternion& orientation);
    void setSpeed(Ogre::Vector3 speed);
    void addSpeed(Ogre::Vector3 speed);
    void addForce(Ogre::Vector3 force);

    float getDensity() const;
    int getSolidity() const;
    void setDensity(float density);
    void setSolidity(int solidity);
    float getWeight();
    Ogre::Entity *getEntity() const;
    Ogre::String getDescription() const;
    void setDescription(Ogre::String description);
    const Ogre::Vector3 getVolume() const;
    std::vector<Ogre::Vector3> getContactSurface(const Ogre::Vector3 normal) const;
    Ogre::Vector3 getCollisionCorrection() const;
    void setCollisionCorrection(Ogre::Vector3 correction);
    bool isOnGround() const;
    Ogre::String getMaterialName() const;
    void setMaterial(const PhysicalMaterial& material);
    void setOriginalMaterial();
    void resetMaterial();

    void addListener(PhysicalObjectListener* listener);
    void removeListener(PhysicalObjectListener* listener);
};

#endif /* PHYSICALOBJECT_H_ */

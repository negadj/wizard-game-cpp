/*
 * PhysicalObject.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "PhysicalObject.h"
#include "../ObjectManager.h"

PhysicalObject::PhysicalObject(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, ObjectType id, Ogre::String meshName,Ogre::Vector3 volume, Ogre::String description) :
	mObjectManager(objectManager),
	mType(id),
	mName(name),
	mDescription(description),
	mNode(originNode->createChildSceneNode(name)),
	mEntity(0),
	mAcceleration(Ogre::Vector3::ZERO),
	mSpeed(Ogre::Vector3::ZERO),
	mSolidity(20),
	mDensity(1),
	mIntegrity(100),
	mVolume(volume),
	mCollisionCorrection(Ogre::Vector3::ZERO),
	mListeners(std::set<PhysicalObjectListener*>())
	//mRemoveOnDestroy(true)
{
	mEntity = mNode->getCreator()->createEntity(mName, meshName);
	mNode->attachObject(mEntity);
}

PhysicalObject::~PhysicalObject() {
#ifdef DEBUG_MODE
LOG("enter PhysicalObject destructor");
#endif
	// destruction des objets créés dans le SceneManager
	if (!mNode->isInSceneGraph()) //On raccroche au scene graphe pour assurer la bonne destruction du noeud
		mNode->getCreator()->getRootSceneNode()->addChild(mNode);
	mNode->getParentSceneNode()->removeAndDestroyChild(mName);
#ifdef DEBUG_MODE
LOG("node deleted");
#endif
	mNode->getCreator()->destroyEntity(mEntity);
#ifdef DEBUG_MODE
LOG("exit PhysicalObject destructor");
#endif
}

bool PhysicalObject::isActive() {
	return mObjectManager->isActive(this);
}

void PhysicalObject::setActive(bool active) {
	mObjectManager->setActive(this, active);
}

void PhysicalObject::doPreCollisionUpdate(Ogre::Real deltaTime)
{
	/* Ajout des frottements */
	addForce(-getSpeed()*getObjectManager()->getStrench(this));
	/* Ajout de la gravité */
	addForce(getObjectManager()->getGravity(this));
}

void PhysicalObject::doPostCollisionUpdate(Ogre::Real deltaTime)
{

}

void PhysicalObject::preCollisionUpdate(Ogre::Real deltaTime)
{
	this->doPreCollisionUpdate(deltaTime);
	/* Integration et mise à jour de la vitesse */
	addSpeed(deltaTime * mAcceleration);
}

void PhysicalObject::postCollisionUpdate(Ogre::Real deltaTime)
{
	this->doPostCollisionUpdate(deltaTime);
	mAcceleration = Ogre::Vector3::ZERO;
}

Ogre::Vector3 PhysicalObject::getAcceleration() const
{
    return mAcceleration;
}

ObjectType PhysicalObject::getObjectType() const
{
    return mType;
}

void PhysicalObject::setObjectType(ObjectType type) {
	mType = type;
}

int PhysicalObject::getIntegrity() const
{
    return mIntegrity;
}

std::string PhysicalObject::getName() const
{
    return mName;
}

Ogre::SceneNode* PhysicalObject::getNode() const
{
    return mNode;
}

Ogre::Vector3 PhysicalObject::getSpeed() const
{
    return mSpeed;
}

void PhysicalObject::setAcceleration(Ogre::Vector3 acceleration)
{
    this->mAcceleration = mAcceleration;
}

void PhysicalObject::setIntegrity(int integrity)
{
	if (integrity == mIntegrity)
		return;
	int oldIntegrity = mIntegrity;
	mIntegrity = std::min(100,std::max(0,integrity)); //borne integrity entre 0 et 100
	onIntegrityChange(oldIntegrity);
}

void PhysicalObject::onIntegrityChange(int oldIntegrity) {
	if (oldIntegrity > 0 and mIntegrity == 0) // En cas de mort
			die();
}

float PhysicalObject::getDensity() const
{
    return mDensity;
}

int PhysicalObject::getSolidity() const
{
    return mSolidity;
}

void PhysicalObject::setDensity(float density)
{
    this->mDensity = density;
}

void PhysicalObject::setEntity(Ogre::Entity *entity)
{
    this->mEntity = entity;
}

Ogre::String PhysicalObject::getDescription() const
{
    return mDescription;
}

ObjectManager* PhysicalObject::getObjectManager() const
{
    return mObjectManager;
}

void PhysicalObject::setDescription(Ogre::String description)
{
    this->mDescription = description;
}

Ogre::Entity *PhysicalObject::getEntity() const
{
    return mEntity;
}

void PhysicalObject::setSolidity(int solidity)
{
    this->mSolidity = solidity;
}

void PhysicalObject::setSpeed(Ogre::Vector3 speed)
{
    this->mSpeed = speed;
}

void PhysicalObject::addSpeed(Ogre::Vector3 speed)
{
    this->mSpeed += speed;
}

const Ogre::Vector3  PhysicalObject::getVolume() const
{
	return mVolume;
}

void PhysicalObject::requestDestruction() {
	mObjectManager->requestDestruction(getName());
}

void PhysicalObject::die() {
	fireDeath();
	requestDestruction();
}

const Ogre::Vector3& PhysicalObject::getPosition() const {
	return mNode->_getDerivedPosition();//convertLocalToWorldPosition(mNode->getPosition());
}

const Ogre::Vector3 PhysicalObject::getFacingDirection() const {
	return getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
}

const Ogre::Quaternion& PhysicalObject::getOrientation() const {
	return mNode->_getDerivedOrientation();//convertLocalToWorldOrientation(mNode->getOrientation());
}

void PhysicalObject::setPosition(const Ogre::Vector3& pos) {
	mNode->_setDerivedPosition(pos);
}

void PhysicalObject::setOrientation(const Ogre::Quaternion& orientation) {
	mNode->_setDerivedOrientation(orientation);
}

Ogre::Vector3 PhysicalObject::getCollisionCorrection() const
{
	return mCollisionCorrection;
}

void PhysicalObject::setCollisionCorrection(Ogre::Vector3 correction)
{
	mCollisionCorrection = correction;
}

bool PhysicalObject::isOnGround() const
{
	Ogre::Vector3 position = getNode()->getPosition();

	if(mObjectManager->isBlockFree(Triplet(round(position.x),floor(position.y - getVolume().y),round(position.z))))
		return false;
	PhysicalObject* obstacle = mObjectManager->getBlock(Triplet(round(position.x),floor(position.y - getVolume().y),round(position.z)));
	return ( 0.01 > position.y - getVolume().y - obstacle->getNode()->getPosition().y - obstacle->getVolume().y);
}

void PhysicalObject::addListener(PhysicalObjectListener* listener) {
	mListeners.insert(listener);
}

void PhysicalObject::removeListener(PhysicalObjectListener* listener) {
	mListeners.erase(listener);
}

void PhysicalObject::fireDeath() const {
#ifdef DEBUG_MODE
LOG("firing Death");
#endif
	for (std::set<PhysicalObjectListener*>::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it) {
		(*it)->objectDied(this);
	}
}

void PhysicalObject::addForce(Ogre::Vector3 force)
{
	mAcceleration += force;
}

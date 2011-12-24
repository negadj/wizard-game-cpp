/*
 * PhysicalObject.cpp
 *
 *  Created on: 30 oct. 2011
 *      Author: gecko
 */

#include "PhysicalObject.h"
#include "../ObjectManager.h"

const Ogre::Vector3 PhysicalObject::LocalDirectionForward = Ogre::Vector3::NEGATIVE_UNIT_Z;
const Ogre::Vector3 PhysicalObject::LocalDirectionBackward = Ogre::Vector3::UNIT_Z;
const Ogre::Vector3 PhysicalObject::LocalDirectionLeft = Ogre::Vector3::NEGATIVE_UNIT_X;
const Ogre::Vector3 PhysicalObject::LocalDirectionRight = Ogre::Vector3::UNIT_X;
const Ogre::Vector3 PhysicalObject::LocalDirectionUp = Ogre::Vector3::UNIT_Y;
const Ogre::Vector3 PhysicalObject::LocalDirectionDown = Ogre::Vector3::NEGATIVE_UNIT_Y;

PhysicalObject::PhysicalObject(ObjectManager* objectManager, Ogre::SceneNode* originNode, Ogre::String name, Ogre::String meshName,Ogre::Vector3 volume, PhysicalMaterial material, Ogre::String description) :
	mObjectManager(objectManager),
	mName(name),
	mTypes(std::set<ObjectType>()),
	mOriginalMeshName(meshName),
	mDescription(description),
	mNode(originNode->createChildSceneNode(name)),
	mEntity(0),
	mAcceleration(Ogre::Vector3::ZERO),
	mSpeed(Ogre::Vector3::ZERO),
	mOriginalMaterial(material),
	mMaterial(material),
	mIntegrity(100),
	mVolume(volume),
	mCollisionCorrection(Ogre::Vector3::ZERO),
	mListeners(std::set<PhysicalObjectListener*>())
{
	mTypes.insert(TYPE_OBJECT);
	mEntity = mNode->getCreator()->createEntity(mName, meshName);
	mNode->attachObject(mEntity);
	setMaterial(material);
}

PhysicalObject::~PhysicalObject() {
#ifdef DEBUG_MODE
LOG("enter PhysicalObject destructor");
#endif
	// destruction des objets créés dans le SceneManager
	if (!mNode->isInSceneGraph()) { //On raccroche au scene graphe pour assurer la bonne destruction du noeud
		mNode->getCreator()->getRootSceneNode()->addChild(mNode);
#ifdef DEBUG_MODE
LOG("node attached to SceneGraph");
#endif
	}
	else
#ifdef DEBUG_MODE
LOG("node already attached to SceneGraph");
#endif
	mNode->getCreator()->destroyEntity(mEntity);
	mNode->removeAndDestroyAllChildren();
	mNode->getCreator()->destroySceneNode(mNode);
#ifdef DEBUG_MODE
LOG("node deleted");
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

bool PhysicalObject::isType(ObjectType type) const
{
    return (mTypes.find(type) != mTypes.end());
}

void PhysicalObject::registerType(ObjectType type) {
	mTypes.insert(type);
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

float PhysicalObject::getWeight() {
	return getDensity() * getVolume().length() * 1000;
}

float PhysicalObject::getDensity() const
{
    return mMaterial.getDensity();
}

int PhysicalObject::getSolidity() const
{
    return mMaterial.getSolidity();
}

void PhysicalObject::setDensity(float density)
{
	mMaterial.setDensity(density);
}

void PhysicalObject::setSolidity(int solidity)
{
    mMaterial.setSolidity(solidity);
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

Ogre::String PhysicalObject::getMaterialName() const {
	return mMaterial.getName();
}

void PhysicalObject::setMaterial(const PhysicalMaterial& material) {
	mMaterial = material;
	if (material.isAffectingLook())
		mEntity->setMaterialName(mMaterial.getName());
	else { // On rétablit l'apparence normale du mesh
		mNode->getCreator()->destroyEntity(mEntity);
		mEntity = mNode->getCreator()->createEntity(mName, mOriginalMeshName);
	}
}

void PhysicalObject::setOriginalMaterial() {
	mOriginalMaterial = mMaterial;
}

void PhysicalObject::resetMaterial() {
	setMaterial(mOriginalMaterial);
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

void PhysicalObject::fireApparenceChanged() const {
#ifdef DEBUG_MODE
LOG("firing Apparence Changed");
#endif
	for (std::set<PhysicalObjectListener*>::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it) {
		(*it)->objectApparenceChanged(this);
	}
}

void PhysicalObject::addForce(Ogre::Vector3 force)
{
	mAcceleration += force;
}

void PhysicalObject::faceDirection(const Ogre::Vector3& direction) {
	getNode()->setDirection(direction,Ogre::Node::TS_WORLD);
}

void PhysicalObject::faceLocalDirection(const Ogre::Vector3& direction) {
	getNode()->setDirection(direction,Ogre::Node::TS_LOCAL);
}

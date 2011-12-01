/*
 * PhysicalObjectListener.h
 *
 *  Created on: 28 nov. 2011
 *      Author: gecko
 */

#ifndef PHYSICALOBJECTLISTENER_H_
#define PHYSICALOBJECTLISTENER_H_

//forward declaration
class PhysicalObject;

/**
 * Listener pour les évènements concernant un PhysicalObject.
 * Note : La liste des évènements déclenchés est pour l'instant très sommaire,
 * mais le mécanisme est en place.
 */
class PhysicalObjectListener {
public:
	PhysicalObjectListener();

	virtual void objectDestroyed(const PhysicalObject* object);
	virtual ~PhysicalObjectListener();
};

#endif /* PHYSICALOBJECTLISTENER_H_ */

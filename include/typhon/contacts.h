#ifndef TYPHON_CONTACTS_H
#define TYPHON_CONTACTS_H

#include "rigidBody.h"

namespace typhon {
	class Contact
	{
	public:

		RigidBody* rb[2];

		real friction;

		real restitution;
		//Position du point de contact dans les coordonnées du monde
		Vector3 contactPoint;
		//Direction du contact dans les coordonnées du monde
		Vector3 contactNormal;
		//Distance d'interpenetration entre les deux entités (au point de contact)
		real penetration;

		void setBodyData(RigidBody first, RigidBody sec, real friction, real restitution);
	};
}

#endif TYPHON_CONTACTS_H
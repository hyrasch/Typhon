#ifndef TYPHON_CONTACTS_H
#define TYPHON_CONTACTS_H

#include "rigidBody.h"

namespace typhon {
	class Contact
	{
	public:
		Vector3 contactPoint;
		Vector3 contactNormal;
		real penetration;

		void setBodyData(RigidBody* first, RigidBody* sec, real friction, real restitution);
	};
}

#endif TYPHON_CONTACTS_H
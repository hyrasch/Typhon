#ifndef TYPHON_RIGIDBODY_H
#define TYPHON_RIGIDBODY_H

#include "core.h"
#include "particle.h"

namespace typhon {
	class RigidBody
	{
	protected:
		real inverseMass;
		real linearDamping;
		// Position et orientation de l'objet
		Vector3 position;
		Quaternion orientation;
		// Vitesse de l'objet (linéaire + angulaire)
		Vector3 velocity;
		Vector3 rotation;
		// Matrice de transformation entre sa base et celle de la simulation
		Matrix4 transform;
	};
}

#endif TYPHON_RIGIDBODY_H
#ifndef TYPHON_BODY_H
#define TYPHON_BODY_H

#include "core.h"

namespace typhon {

	class RigidBody
	{
	public:
		// Informations dans l'espace (world space)
		real inverseMass; // Masse inverse
		Vector3 position; // Position
		Quaternion orientation; // Orientation

		// Informations de déplacement (world space)
		Vector3 velocity; // Vélocité linéaire
		Vector3 rotation; // Vélocité angulaire
		Vector3 acceleration; // Accélération linéaire
		Vector3 lastFrameAcceleration; // Dernière accélération

		// Matrice4 de transformation (body space <==> world space)
		Matrix4 transformMatrix;

		// Tenseurs 
		Matrix3 inverseInertiaTensor; // Tenseur d'intertie inverse (body space)
		Matrix3 inverseInertiaTensorWorld; // Tenseur d'intertie inverse (world space)

		// Accumulateurs à appliquer à l'intégration
		Vector3 forceAccum; // Somme des forces
		Vector3 torqueAccum; // Somme des couples

		// Amortissements
		real linearDamping; // Ammortissement linéaire
		real angularDamping; // Ammortissement angulaire


		void calculateInertiaTensorWS();
		void integrate(real duration);
		bool hasFiniteMass() const;
		void clearAccumulators();

		void addForce(const Vector3& force);
		void addForceAtPoint(const Vector3& force, const Vector3& point);
		void addTorque(const Vector3& torque);

		// Getters & Setters
#pragma region Get & Set

		void setMass(const real mass);
		real getMass() const;

		void setInverseMass(const real inverseMass);
		real getInverseMass() const;

		void setInverseInertiaTensor(const Matrix3& inverseInertiaTensor);
		void getInverseInertiaTensor(Matrix3* inverseInertiaTensor) const;
		Matrix3 getInverseInertiaTensor() const;

		void setDamping(const real linearDamping, const real angularDamping);
		void setLinearDamping(const real linearDamping);
		real getLinearDamping() const;
		void setAngularDamping(const real angularDamping);
		real getAngularDamping() const;

		void setPosition(const real x, const real y, const real z);
		Vector3 getPosition() const;

		void setOrientation(const real r, const real i, const real j, const real k);
		Quaternion getOrientation() const;

		Matrix4 getTransform() const;

		Vector3 getPointInWorldSpace(const Vector3& point) const;

		void setVelocity(const real x, const real y, const real z);
		Vector3 getVelocity() const;

		void setRotation(const real x, const real y, const real z);
		Vector3 getRotation() const;

		void setAcceleration(const real x, const real y, const real z);
		Vector3 getAcceleration() const;
#pragma endregion
	};
}

#endif TYPHON_BODY_H

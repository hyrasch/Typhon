#ifndef TYPHON_FGEN_H
#define TYPHON_FGEN_H

#include "rigidBody.h"
#include "particleForceGen.h"
#include <vector>

namespace typhon {

	// Générateur de forces pour les corps
	class ForceGenerator
	{
	public:
		virtual void updateForce(RigidBody* body, real duration) = 0;
	};

	class ForceRegistry
	{
	public:
		struct ForceRegistration {
			RigidBody* body;
			ForceGenerator* fg;
		};

	public:
		typedef std::vector<ForceRegistration> Registry;
		Registry registrations;

	public:
		void add(RigidBody* body, ForceGenerator* fg);

		void updateForces(real duration);
	};

	class Gravity : public ForceGenerator
	{
	private:
		Vector3 gravity;

	public:
		Gravity(const Vector3& gravity) : gravity(gravity) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	class Spring : public ForceGenerator
	{
	private:
		Vector3 connectionPoint;
		Vector3 otherConnectionPoint;

		RigidBody* other;

		real springConstant;
		real restLength;

	public:
		Spring(const Vector3& localConnectionPt, RigidBody* other, const Vector3& otherConnectionPt, real springConstant, real restLength)
			: connectionPoint(localConnectionPt), otherConnectionPoint(otherConnectionPt),
			other(other), springConstant(springConstant), restLength(restLength) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	class RotationCCW : public ForceGenerator
	{
	private:
		Matrix3 tensor;

		Vector3 position;
	public:

		RotationCCW(const Matrix3 tensor, const Vector3 position) : tensor(tensor), position(position) {}

		virtual void updateForce(RigidBody* body, real duration);
	};
	
	class RotationCW : public ForceGenerator
	{
	private:
		Matrix3 tensor;

		Vector3 position;
	public:

		RotationCW(const Matrix3 tensor, const Vector3 position) : tensor(tensor), position(position) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	class Trampoline : public ForceGenerator
	{
	private:
		Matrix3 tensor;

		Vector3 torque;

	public:

		Trampoline(const Matrix3 tensor, const Vector3 torque) : tensor(tensor), torque(torque) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	class Carambolage : public ForceGenerator
	{
	private:
		Matrix3 tensor;

		int id;
	public:

		Carambolage(const Matrix3 tensor, const int id) : tensor(tensor), id(id) {}

		virtual void updateForce(RigidBody* body, real duration);
	};
}

#endif	TYPHON_FGEN_H
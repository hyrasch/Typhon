#ifndef TYPHON_FGEN_H
#define TYPHON_FGEN_H

#include "body.h"
#include "particleForceGen.h"
#include <vector>

namespace typhon {

	class ForceGenerator
	{
	public:

		virtual void updateForce(RigidBody* body, real duration) = 0;
	};

    class ForceRegistry
    {
    protected:

        struct ForceRegistration
        {
            RigidBody* body;
            ForceGenerator* fg;
        };

        typedef std::vector<ForceRegistration> Registry;
        Registry registrations;

    public:

        void add(RigidBody* body, ForceGenerator* fg);

        void updateForces(real duration);
    };

	class Gravity : public ForceGenerator
	{
		Vector3 gravity;
	public:

		Gravity(const Vector3& gravity) : gravity(gravity) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	class Spring : public ForceGenerator
	{

		Vector3 connectionPoint;

		Vector3 otherConnectionPoint;

		RigidBody* other;

		real springConstant;

		real restLength;
	public:

		Spring(const Vector3& localConnectionPt, RigidBody* other, const Vector3& otherConnectionPt, real springConstant, real restLength) 
			: connectionPoint(localConnectionPt), otherConnectionPoint(otherConnectionPt), other(other), springConstant(springConstant), restLength(restLength) {}

		virtual void updateForce(RigidBody* body, real duration);
	};

	/*class CarForce : public ForceGenerator
	{
		Matrix3 tensor;

		Vector3 position;

		const Vector3* speed;
	public:

		CarForce(const Matrix3& tensor, const Vector3& position, const Vector3* speed) : tensor(tensor), position(position), speed(speed) {}

		virtual void updateForce(RigidBody* body, real duration);

		void updateForceFromTensor(RigidBody* body, real duration, const Matrix3& tensor);

	};*/



}

#endif	TYPHON_FGEN_H
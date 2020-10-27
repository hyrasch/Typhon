#ifndef TYPHON_PFGEN_H
#define TYPHON_PFGEN_H

#include "core.h"
#include "particle.h"

#include <vector>

namespace typhon{

	class ParticleForceGenerator
	{
	public:

		virtual void updateForce(Particle* particle, real duration) = 0;
	};

	class ParticleForceRegistry
	{
	protected:

		struct ParticleForceRegistration
		{
			Particle* particle;
			ParticleForceGenerator* fg;
		};

		typedef std::vector<ParticleForceRegistration> Registry;
		Registry registrations;
	public:

		void add(Particle* particle, ParticleForceGenerator* fg);

		void remove(Particle* particle, ParticleForceGenerator* fg);

		void clear();

		void updateForces(real duration);
	};

	//----------------------- Gravity --------------------------------------
	//
	//
	//----------------------------------------------------------------------
	class ParticleGravity : public ParticleForceGenerator
	{

		Vector3 gravity;
	public:

		ParticleGravity(const Vector3& gravity) : gravity(gravity) {}

		virtual void updateForce(Particle* particle, real duration);
	};

	//----------------------- Drag -----------------------------------------
	//
	//
	//----------------------------------------------------------------------
	class ParticleDrag : public ParticleForceGenerator
	{

		real k1;

		real k2;
	public:

		ParticleDrag(real k1, real k2) : k1(k1), k2(k2) {}

		virtual void updateForce(Particle* particle, real duration);
	};

	//----------------------- Spring --------------------------------------
	//
	//
	//---------------------------------------------------------------------
	class ParticleSpring : public ParticleForceGenerator
	{
		Particle* other;

		real springConstant;

		real restLength;
	public:

		ParticleSpring(Particle* other, real springConstant, real restLength) : other(other), springConstant(springConstant), restLength(restLength) {}

		virtual void updateForce(Particle* particle, real duration);
	};

	//----------------------- AnchoredSpring ------------------------------
	//
	//
	//---------------------------------------------------------------------
	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
		Vector3* anchor;

		real springConstant;

		real restLength;
	public:

		ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength) : anchor(anchor), springConstant(springConstant), restLength(restLength) {}

		virtual void updateForce(Particle* particle, real duration);
	};

	//----------------------- Bungee --------------------------------------
	//
	//
	//---------------------------------------------------------------------
	class ParticleBungee : public ParticleForceGenerator
	{
		Particle* other;

		real springConstant;

		real restLength;
	public:

		ParticleBungee(Particle* other, real springConstant, real restLength) : other(other), springConstant(springConstant), restLength(restLength) {}

		virtual void updateForce(Particle* particle, real duration);
	};

	//----------------------- Buoyancy ------------------------------------
	//
	//
	//---------------------------------------------------------------------
	class ParticleBuoyancy : public ParticleForceGenerator
	{
		real maxDepth;

		real volume;

		real waterHeight;

		real liquidDensity;
	public:

		ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f) : maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {}

		virtual void updateForce(Particle* particle, real duration);
	};

	//----------------------- FakeSpring ----------------------------------
	//
	//
	//---------------------------------------------------------------------
	class ParticleFakeSpring : public ParticleForceGenerator
	{
		Vector3* anchor;

		real springConstant;

		real damping;
	public:

		ParticleFakeSpring(Vector3* anchor, real springConstant, real damping) : anchor(anchor), springConstant(springConstant), damping(damping) {}

		virtual void updateForce(Particle* particle, real duration);
	};

}

#endif // !TYPHON_PFGEN_H
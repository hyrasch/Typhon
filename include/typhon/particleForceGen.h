#ifndef TYPHON_PFGEN_H
#define TYPHON_PFGEN_H

#include "core.h"
#include "particle.h"

#include <vector>

namespace typhon {

	// Force generator
	class ParticleForceGenerator {
	public:
		virtual void updateForce(Particle* particle, real duration) = 0;
	};

	// Registry for forces
	class ParticleForceRegistry {
	public:
		// Each registry has a particle and a force generator
		struct ParticleForceRegistration {
			Particle* particle;
			ParticleForceGenerator* fg;
		};

		std::vector<ParticleForceRegistration> registrations;

	public:
		// Creating register
		int add(Particle* particle, ParticleForceGenerator* fg);
		// Deleting register
		void remove(Particle* particle, int index);
		// Clearing registry
		void clear();
		// Updating forces applied to a particle
		void updateForces(real duration);
	};

	// =============================================
	// Gravity
	class ParticleGravity : public ParticleForceGenerator {
	private:
		Vector3 gravity;

	public:
		ParticleGravity(const Vector3& gravity) : gravity(gravity) {}
		virtual void updateForce(Particle* particle, real duration);
	};

	// =============================================
	// Drag
	class ParticleDrag : public ParticleForceGenerator {
	private:
		// Drag constants
		const real k1;
		const real k2;

	public:
		ParticleDrag(real k1, real k2) : k1(k1), k2(k2) {}
		virtual void updateForce(Particle* particle, real duration);
	};

	// =============================================
	// Spring
	class ParticleSpring : public ParticleForceGenerator {
	private:
		// The other particle attached
		Particle* other;
		// Spring constant
		real springConstant;
		// Rest length of the spring
		real restLength;

	public:
		ParticleSpring(Particle* other, real springConstant, real restLength)
			: other(other), springConstant(springConstant), restLength(restLength) {}
		virtual void updateForce(Particle* particle, real duration);
	};

	// =============================================
	// Anchored spring
	class ParticleAnchoredSpring : public ParticleForceGenerator {
	private:
		// Position of the anchor
		Vector3* anchor;
		// Spring constant
		real springConstant;
		// Rest length of the spring
		real restLength;

	public:
		ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength)
			: anchor(anchor), springConstant(springConstant), restLength(restLength) {}
		virtual void updateForce(Particle* particle, real duration);
	};

	// =============================================
	// Bungee
	class ParticleBungee : public ParticleForceGenerator {
	private:
		// The other particle attached
		Particle* other;
		// Spring constant
		real springConstant;
		// Rest length of the spring
		real restLength;

	public:
		ParticleBungee(Particle* other, real springConstant, real restLength)
			: other(other), springConstant(springConstant), restLength(restLength) {}
		virtual void updateForce(Particle* particle, real duration);
	};

	// =============================================
	// Buoyancy
	class ParticleBuoyancy : public ParticleForceGenerator {
	private:
		// The level at which the buoyancy effect is max
		real maxDepth;
		// The water volume
		real volume;
		// The surface height of the water
		real waterHeight;
		// The density of the water
		real liquidDensity;

	public:
		ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f)
			: maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {}
		virtual void updateForce(Particle* particle, real duration);
	};

	// =============================================
	// Fake spring
	class ParticleFakeSpring : public ParticleForceGenerator {
	private:
		// The position of the anchor on which the spring is attached
		Vector3* anchor;
		// Spring constant
		real springConstant;
		// Damping (constant)
		real damping;

	public:
		ParticleFakeSpring(Vector3* anchor, real springConstant, real damping)
			: anchor(anchor), springConstant(springConstant), damping(damping) {}
		virtual void updateForce(Particle* particle, real duration);
	};

	// =============================================
	// Blob
	class BlobForceGenerator : public ParticleForceGenerator {
	private:
		// All particles in the simulation
		Particle* particles;
		// Repulsion force
		real repulsion;
		// Attraction force
		real attraction;
		// Min value when to apply max repulsion
		real minRepulsDist;
		// Max value when to apply attraction
		real maxAttracDist;
		// Max value when to apply a cable-like force
		real cableDist;

	public:
		BlobForceGenerator(Particle* particles, real repulsion, real maxAttraction, real minRepulsDist, real maxAttracDist, real cableDist)
			: particles(particles), repulsion(repulsion), attraction(attraction),
			minRepulsDist(minRepulsDist), maxAttracDist(maxAttracDist), cableDist(cableDist) {}

		virtual void updateForce(Particle* particle, real duration);
	};

}

#endif TYPHON_PFGEN_H
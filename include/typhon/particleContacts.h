#ifndef TYPHON_PCONTACTS_H
#define TYPHON_PCONTACTS_H

#include "particle.h"

namespace typhon {

	class ParticleContactResolver;

	class ParticleContact {
		// We need protected member variables from the Resolver
		friend class ParticleContactResolver;

	public:
		// Both particle invoclved
		Particle* particle[2];
		// Normal restition coefficient
		real restitution;
		// Direction of the contact
		Vector3 contactNormal;
		// Penetration between 2 particles
		real penetration;

	protected:
		// Resolving the contact
		void resolve(real duration);
		// Outputs the velocity that separates both particles
		real calculateSeparatingVelocity() const;

	private:
		// Resolving the velocity of the particles
		void resolveVelocity(real duration);
		// Resolving the "layering" of the particles
		void resolveInterpenetration(real duration);
	};

	// Contact resolver, doing the routine
	class ParticleContactResolver {
	protected:
		// Max iterations per frame
		unsigned iterations;
		// Iterations used per frame
		unsigned iterationsUsed;

	public:
		// Constructor
		ParticleContactResolver(unsigned iterations) : iterations(iterations), iterationsUsed(0) {}
		// Setting up the iterations
		void setIterations(unsigned iterations);
		// Resolving contacts (velocity + interpenetration) between 2 particles
		void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
	};

	// Contact generator
	class ParticleContactGenerator {
	public:
		// Adding a new contact
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
	};
}

#endif TYPHON_PCONTACTS_H
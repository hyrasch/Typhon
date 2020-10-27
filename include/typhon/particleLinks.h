#ifndef TYPHON_PLINKS_H
#define TYPHON_PLINKS_H

#include "particleContacts.h"

namespace typhon {

	class ParticleLink : ParticleContactGenerator {
	public:
		// A pair of particle
		Particle* particle[2];

	protected:
		// The length of the link between both particles
		real currentLength() const;

	public:

		// Generates a contact to ensure that the link is preserved
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
	};

	// =============================================
	// Cable
	class ParticleCable : public ParticleLink {
	public:
		// The max length of the cable
		real maxLength;
		// The restitution of the cable when the ends are too far
		real restitution;

		// Creating a contact so that the cable does not overextends
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const override;
	};

	// =============================================
	// Rod
	class ParticleRod : public ParticleLink {
	public:
		// The length of the rod
		real length;

		// Creating a contact so that the ends stand at the same distance
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const override;
	};
}

#endif TYPHON_PLINKS_H
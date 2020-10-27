#ifndef TYPHON_PWORLD_H
#define TYPHON_PWORLD_H

#include "particleForceGen.h"
#include "particleLinks.h"

namespace typhon {

	class ParticleWorld {
	private:
		struct ParticleRegistration {
			Particle* particle;
			ParticleRegistration* next;
		};

		ParticleRegistration* firstParticle;

	public:
		ParticleWorld(unsigned maxContacts, unsigned iterations = 0);
		void startFrame();
		unsigned generateContacts();
		void integrate(real duration);
		void runPhysics(real duration);
		bool calculateIterations;

	protected:
		struct ContactGenRegistration {
			ParticleContactGenerator* gen;
			ContactGenRegistration* next;
		};

		ParticleForceRegistry registry;
		ParticleContactResolver resolver;
		ContactGenRegistration* firstContactGen;
		ParticleContact* contacts;
		unsigned maxContacts;
	};
}

#endif TYPHON_PWORLD_H

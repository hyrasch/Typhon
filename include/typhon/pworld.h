#ifndef TYPHON_PWORLD_H
#define TYPHON_PWORLD_H

#include "pfgen.h"
#include "plinks.h"

namespace typhon {

	class ParticleWorld
	{
		struct ParticleRegistration
		{
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
		ParticleForceRegistry registry;

		ParticleContactResolver resolver;

		struct ContactGenRegistration
		{
			ParticleContactGenerator* gen;
			ContactGenRegistration* next;
		};

		ContactGenRegistration* firstContactGen;

		ParticleContact* contacts;

		unsigned maxContacts;
			
	};

}
#endif // !TYPHON_PWORLD_H

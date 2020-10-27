#ifndef TYPHON_PWORLD_H
#define TYPHON_PWORLD_H

#include "particleForceGen.h"
#include "particleContacts.h"
#include "particleLinks.h"

namespace typhon {

#define NB_PARTICLES 2
#define PARTICLE_RADIUS 1.0

	class ParticleWorld {
	public:
		Particle* particles;
		ParticleForceRegistry registry;
		std::vector<ParticleContactGenerator*> contactGenerators;

	public:
		ParticleWorld(unsigned iterations = 0);
		void startFrame();
		unsigned generateContacts();
		void integrate(real duration);
		void runPhysics(real duration);
		bool calculateIterations;

	protected:

		ParticleContactResolver resolver;
		ParticleContact* contacts;
		unsigned maxContacts;
	};

	class Platform : public ParticleContactGenerator {
	public:
		const real xMin = -50.0;
		const real xMax = 50.0;
		const real zMin = -50.0;
		const real zMax = 50.0;

	public:
		Particle* particles;

		// Hérité via ParticleContactGenerator
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const override;
	};

	class BlobForceGenerator : public ParticleForceGenerator
	{
	private:
		Particle* particles;
		real maxRepulsion;
		real maxAttraction;
		real minNaturalDistance;
		real maxNaturalDistance;
		real floatHead;
		unsigned maxFloat;
		real maxDistance;
	public:
		
		BlobForceGenerator(Particle* particles, real maxRepulsion, real maxAttraction, real minNaturalDistance, real maxNaturalDistance, real floatHead, unsigned maxFloat, real maxDistance)
			: particles(particles), maxRepulsion(maxRepulsion), maxAttraction(maxAttraction), minNaturalDistance(minNaturalDistance), maxNaturalDistance(maxNaturalDistance), floatHead(floatHead), maxFloat(maxFloat), maxDistance(maxDistance) {}
		virtual void updateForce(Particle* particle, real duration);

	};
}

#endif TYPHON_PWORLD_H

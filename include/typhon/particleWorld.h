#ifndef TYPHON_PWORLD_H
#define TYPHON_PWORLD_H

#include "particleForceGen.h"
#include "particleContacts.h"
#include "particleLinks.h"

namespace typhon {

#define NB_PARTICLES 10
#define PARTICLE_RADIUS 0.4

	// Holding our simulation
	class ParticleWorld {
	public:
		// Array of particles simulated
		Particle* particles;
		// The registry for our forces applied to the particles
		ParticleForceRegistry registry;
		// The holder of the contacts generated in our simulation
		std::vector<ParticleContactGenerator*> contactGenerators;

	public:
		ParticleWorld(unsigned iterations = 0);
		// Clearing particles accum
		void startFrame();
		// Checking contacts that happens in the frame
		unsigned generateContacts();
		// Integration
		void integrate(real duration);
		// The routine of the simulation
		// ==> Forces, contacts, integration, 
		void runPhysics(real duration);
		// Iterations are fixes or not
		bool calculateIterations;

	protected:
		// Contact esolver
		ParticleContactResolver resolver;
		// Contact array
		ParticleContact* contacts;
		// Max contacts handled in one frame
		unsigned maxContacts;
	};
}

#endif TYPHON_PWORLD_H

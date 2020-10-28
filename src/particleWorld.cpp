#include <typhon/particleWorld.h>

using namespace typhon;

ParticleWorld::ParticleWorld(unsigned iterations) : resolver(iterations), maxContacts(NB_PARTICLES) {
	particles = new Particle[NB_PARTICLES];
	ParticleGravity* pg = new ParticleGravity(Vector3::HIGH_GRAVITY);
	BlobForceGenerator* bfg = new BlobForceGenerator(particles, 40.0, 20.0, PARTICLE_RADIUS * 0.5, PARTICLE_RADIUS * 2, PARTICLE_RADIUS * 5);


	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		particles[i].setDamping(0.2);
		particles[i].setAcceleration(0, 0, 0);
		particles[i].setVelocity(0, 0, 0);
		if (i >= NB_PARTICLES / 2)
			particles[i].setPosition(0, 10, PARTICLE_RADIUS * 5 * i);
		else
			particles[i].setPosition(-10, 10, PARTICLE_RADIUS * -5 * i);
		if (i == 0)
			particles[i].setInverseMass(0.8);
		else if (i >= NB_PARTICLES / 2)
			particles[i].setInverseMass(0.4);
		else
			particles[i].setInverseMass(1);
		particles[i].clearAccumulator();

		registry.add(particles + i, pg);
		if (i != 0) {
			registry.add(particles + i, bfg);
		}
	}

	Platform* platform = new Platform();
	platform->particles = particles;

	contactGenerators.push_back(platform);

	contacts = new ParticleContact[maxContacts];
	calculateIterations = (iterations == 0);

}

void ParticleWorld::startFrame() {
	for (unsigned i = 0; i < NB_PARTICLES; i++)
		particles[i].clearAccumulator();
}

unsigned ParticleWorld::generateContacts() {
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;

	for (ParticleContactGenerator* pcg : contactGenerators) {
		unsigned used = pcg->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0) break;
	}

	return maxContacts - limit;
}
void ParticleWorld::integrate(real duration)
{
	for (unsigned i = 0; i < NB_PARTICLES; i++)
		particles[i].integrate(duration);

}

void ParticleWorld::runPhysics(real duration) {
	registry.updateForces(duration);

	integrate(duration);

	unsigned usedContacts = generateContacts();

	if (calculateIterations) resolver.setIterations(usedContacts * 2);
	resolver.resolveContacts(contacts, usedContacts, duration);
}

// =================================
// Specific to the TP2 simulation

unsigned Platform::addContact(ParticleContact* contact, unsigned limit) const {
	// 0 => no rebound
	// 1 => constant rebound
	const static real restitution = 0.8;

	unsigned used = 0;

	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		if (used >= limit) break;

		bool underGround = false;
		if (particles[i].getPosition().y < 0) { underGround = true; }

		// Getting the bottom of the particle
		real height = particles[i].getPosition().y - PARTICLE_RADIUS;

		if (height > 0) return used;

		// Knowing where we are
		particles[i].setGround();
		particles[i].setFlaque();

		// If we're on the ground
		if (particles[i].getGround() && !underGround) {
			contact->contactNormal = Vector3::UP;
			contact->penetration = height;
			contact->restitution = restitution;

			contact->particle[0] = particles + i;
			// This is the ground
			contact->particle[1] = nullptr;

			used++;
			contact++;
		}
	}

	return used;
}

void BlobForceGenerator::updateForce(Particle* particle, real duration) {
	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		// If it's the same particle, or if it's underground
		if (particles + i == particle || particle->getPosition().y < 0) continue;

		// Direction + magnitude of the vector
		Vector3 separation = particles[i].getPosition() - particle->getPosition();
		real distance = separation.magnitude();

		// Repulsion
		if (distance < minRepulsDist) {
			distance = PARTICLE_RADIUS - distance / minRepulsDist;
			particle->addForce(separation.unit() * (1.0 - distance) * repulsion * -1.0f);
		}
		// Attraction
		else if (distance > maxAttracDist && distance < cableDist) {
			distance = (distance - maxAttracDist) / (cableDist - maxAttracDist);
			particle->addForce(separation.unit() * distance * attraction);
		}
		// Cable
		else {
			Vector3 force;
			particle->getPosition(&force);
			// Attach to the head
			Particle* other = particles;
			force -= other->getPosition();

			real magnitude = force.magnitude();
			magnitude = real_abs(magnitude - cableDist);
			magnitude *= 2;

			force.normalise();
			force *= -magnitude;

			particle->addForce(force);
		}
	}
}

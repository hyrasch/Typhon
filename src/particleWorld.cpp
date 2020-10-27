#include <typhon/particleWorld.h>

using namespace typhon;

ParticleWorld::ParticleWorld(unsigned iterations) : resolver(iterations), maxContacts(NB_PARTICLES) {
	particles = new Particle[NB_PARTICLES];
	ParticleGravity* pg = new ParticleGravity(Vector3::GRAVITY);

	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		particles[i].setDamping(0.2);
		particles[i].setAcceleration(Vector3::GRAVITY);
		particles[i].setVelocity(0, 0, 0);
		particles[i].setPosition(0, 15, 0);
		particles[i].setInverseMass(1);
		particles[i].clearAccumulator();

		registry.add(particles + i, pg);
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

unsigned Platform::addContact(ParticleContact* contact, unsigned limit) const {
	const static real restitution = 0.8;

	unsigned used = 0;

	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		if (used >= limit) break;

		bool underGround = false;

		if (particles[i].getPosition().y < 0) { underGround = true; }

		real heigth = particles[i].getPosition().y - PARTICLE_RADIUS;

		if (heigth > 0) return used;

		if (particles[i].getPosition().x <= xMax && particles[i].getPosition().x >= xMin
			&& particles[i].getPosition().z <= zMax && particles[i].getPosition().z >= zMin && particles[i].getPosition().y && !underGround) {

			contact->contactNormal = Vector3::UP;
			contact->penetration = heigth;
			contact->restitution = restitution;

			contact->particle[0] = particles + i;
			contact->particle[1] = nullptr;

			used++;
			contact++;
		}
	}

	return used;
}

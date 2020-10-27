#include <typhon/particleWorld.h>

using namespace typhon;

ParticleWorld::ParticleWorld(unsigned iterations) : resolver(iterations), maxContacts(NB_PARTICLES) {
	particles = new Particle[NB_PARTICLES];
	ParticleGravity* pg = new ParticleGravity(Vector3::GRAVITY);
	BlobForceGenerator* bfg = new BlobForceGenerator(particles, 20.0, 10.0, PARTICLE_RADIUS * 0.75, PARTICLE_RADIUS * 1.5, PARTICLE_RADIUS * 3, 2, 8.0);


	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		particles[i].setDamping(0.2);
		particles[i].setAcceleration(Vector3::GRAVITY);
		particles[i].setVelocity(0, 0, 0);
		particles[i].setPosition(10*i, 15, 0);
		particles[i].setInverseMass(1);
		particles[i].clearAccumulator();

		registry.add(particles + i, pg);
		registry.add(particles + i, bfg);
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

		real height = particles[i].getPosition().y - PARTICLE_RADIUS;

		if (height > 0) return used;

		if (particles[i].getPosition().x <= xMax && particles[i].getPosition().x >= xMin
			&& particles[i].getPosition().z <= zMax && particles[i].getPosition().z >= zMin && !underGround) {

			contact->contactNormal = Vector3::UP;
			contact->penetration = height;
			contact->restitution = restitution;

			contact->particle[0] = particles + i;
			contact->particle[1] = nullptr;

			used++;
			contact++;

		}
	}

	return used;
}

void BlobForceGenerator::updateForce(Particle* particle, real duration)
{
    unsigned joinCount = 0;
    for (unsigned i = 0; i < NB_PARTICLES; i++)
    {
        // Don't attract yourself
        if (particles + i == particle) continue;

        // Work out the separation distance
        typhon::Vector3 separation =
            particles[i].getPosition() - particle->getPosition();
        separation.z = 0.0f;
        real distance = separation.magnitude();

        if (distance < minNaturalDistance)
        {
            // Use a repulsion force.
            distance = 1.0f - distance / minNaturalDistance;
            particle->addForce(
                separation.unit() * (1.0f - distance) * maxRepulsion * -1.0f
            );
            joinCount++;
        }
        else if (distance > maxNaturalDistance && distance < maxDistance)
        {
            // Use an attraction force.
            distance =
                (distance - maxNaturalDistance) /
                (maxDistance - maxNaturalDistance);
            particle->addForce(
                separation.unit() * distance * maxAttraction
            );
            joinCount++;
        }
    }

    if (particle == particles && joinCount > 0 && maxFloat > 0)
    {
        real force = real(joinCount / maxFloat) * floatHead;
        if (force > floatHead) force = floatHead;
        particle->addForce(typhon::Vector3(0, force, 0));
    }

}

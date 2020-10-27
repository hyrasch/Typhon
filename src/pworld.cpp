#include <typhon/pworld.h>

using namespace typhon;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
	:
	resolver(iterations),
	maxContacts(maxContacts)
{
	contacts = new ParticleContact[maxContacts];
	calculateIterations = (iterations == 0);

}

void ParticleWorld::startFrame()
{
	ParticleRegistration* reg = firstParticle;
	while (reg)
	{
		reg->particle->clearAccumulator();
		reg = reg->next;
	}
}

unsigned ParticleWorld::generateContacts()
{
		unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;
	ContactGenRegistration* reg = firstContactGen;
	while (reg)
	{
		unsigned used = reg->gen->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0) break;
		reg = reg->next;
	}

	return maxContacts - limit;
}
void ParticleWorld::integrate(real duration)
{
	ParticleRegistration* reg = firstParticle;
	while (reg)
	{
		reg->particle->integrate(duration);

		reg = reg->next;
	}
}
void ParticleWorld::runPhysics(real duration)
{
	registry.updateForces(duration);

	integrate(duration);

	unsigned usedContacts = generateContacts();

	if (calculateIterations) resolver.setIterations(usedContacts * 2);
	resolver.resolveContacts(contacts, usedContacts, duration);
}
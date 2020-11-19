#include <cstdlib>
#include <typhon/world.h>

using namespace typhon;

void World::startFrame()
{
	BodyRegistration* reg = firstBody;
	while (reg)
	{
		// Remove all forces from the accumulator.
		reg->body->clearAccumulators();
		reg->body->calculateDerivedData();
		// Get the next registration.
		reg = reg->next;
	}
}

void World::runPhysics(real duration)
{

	BodyRegistration * reg = firstBody;
	while (reg)
	{
		// Remove all forces from the accumulator
		reg->body->integrate(duration);
		// Get the next registration
		reg = reg->next;
	}
}
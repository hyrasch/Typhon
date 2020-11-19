#include <cstdlib>
#include <typhon/world.h>

using namespace typhon;

void World::startFrame()
{
	BodyRegistration* reg = firstBody;
	while (reg)
	{
		reg->body->clearAccumulators();
		reg->body->calculateDerivedData();

		reg = reg->next;
	}
}

void World::runPhysics(real duration)
{
	BodyRegistration * reg = firstBody;
	while (reg)
	{
		reg->body->integrate(duration);

		reg = reg->next;
	}
}
#include <cstdlib>
#include <iostream>
#include <typhon/world.h>
#include <math.h>

using namespace typhon;

World::World()
{
	//Inititalisation de la voiture 1
	myCar.reset(-20);

	//Inititalisation de la voiture 2
	myCar2.reset(20);
}

void World::startFrame() 
{
	BodyRegistration* reg = firstBody;

	while (reg) {
		reg->body->clearAccumulators();
		reg->body->calculateInertiaTensorWS();

		reg = reg->next;
	}
}

void World::runPhysics(real duration) 
{
	BodyRegistration* reg = firstBody;

	while (reg) {
		reg->body->integrate(duration);

		reg = reg->next;
	}
}

void World::Car::reset(int z)
{
	body.setMass(2.5f);
	body.setDamping(0.8f, 0.8f);
	body.setPosition(0, 2, z);
	body.setOrientation(1, 0, 0, 0);
	body.setRotation(0, 0, 0);
	body.setAcceleration(0, 0, 0);
	body.setVelocity(0, 0, 0);

	registry.registrations.clear();

	body.setInverseInertiaTensor(Matrix3(body.getMass() / 24.0f, 0, 0, 0, body.getMass() / 24.0f, 0, 0, 0, body.getMass() / 24.0f));
	body.calculateInertiaTensorWS();
}

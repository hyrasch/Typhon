#include <cstdlib>
#include <iostream>
#include <typhon/world.h>
#include <math.h>

using namespace typhon;

World::World()
{
	myCar.body.setMass(2.5f);
	myCar.body.setDamping(0.8f, 0.8f);
	myCar.body.setPosition(0, 20, 0);
	myCar.body.setOrientation(1, 0, 0, 0);
	myCar.body.setRotation(0, 0, 0);

	myCar.body.setInverseInertiaTensor(Matrix3(myCar.body.getMass() / 24.0f, 0, 0, 0, myCar.body.getMass() / 24.0f, 0, 0, 0, myCar.body.getMass() / 24.0f));

	myCar.body.calculateInertiaTensorWS();


	
}

void World::startFrame() {
	BodyRegistration* reg = firstBody;

	while (reg) {
		reg->body->clearAccumulators();
		reg->body->calculateInertiaTensorWS();

		reg = reg->next;
	}
}

void World::runPhysics(real duration) {
	BodyRegistration* reg = firstBody;

	while (reg) {
		reg->body->integrate(duration);

		reg = reg->next;
	}
}

void World::Car::updatePos(World world)
{
	frontUpperLeft = massCenter - Vector3(0.5, -0.5, 0.5);
	frontUpperRight = massCenter - Vector3(0.5, -0.5, -0.5);
	frontLowerRight = massCenter - Vector3(0.5, 0.5, -0.5);
	frontLowerLeft = massCenter - Vector3(0.5, 0.5, 0.5);
	backUpperLeft = massCenter - Vector3(-0.5, -0.5, 0.5);
	backUpperRight = massCenter - Vector3(-0.5, -0.5, -0.5);
	backLowerRight = massCenter - Vector3(-0.5, 0.5, -0.5);
	backLowerLeft = massCenter - Vector3(-0.5, 0.5, 0.5);

	massCenter = world.myCar.body.getPosition();
}
#include <typhon/world.h>

using namespace typhon;

World::World() {
	//Inititalisation de la voiture 1
	myCar.reset(-20);

	//Inititalisation de la voiture 2
	myCar2.reset(20);
}

void World::Car::reset(int z) {
	body.setMass(2.f);
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

void World::Update(real duration) {
	myCar.body.clearAccumulators();
	myCar.registry.updateForces(duration);
	myCar.body.integrate(duration);

	myCar2.body.clearAccumulators();
	myCar2.registry.updateForces(duration);
	myCar2.body.integrate(duration);
}
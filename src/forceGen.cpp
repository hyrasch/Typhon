#include <typhon/forceGen.h>

using namespace typhon;

void ForceRegistry::updateForces(real duration) {
	Registry::iterator i = registrations.begin();

	for (; i != registrations.end(); i++)
		i->fg->updateForce(i->body, duration);

	/*for (ForceRegistration r : registrations) {
		r.fg->updateForce(r.body, duration);
	}*/
}

void ForceRegistry::add(RigidBody* body, ForceGenerator* fg) {
	ForceRegistry::ForceRegistration registration;

	registration.body = body;
	registration.fg = fg;
	registrations.push_back(registration);
}

void Gravity::updateForce(RigidBody* body, real duration) {
	if (!body->hasFiniteMass()) return;

	body->addForce(gravity * body->getMass());
}

void Spring::updateForce(RigidBody* body, real duration) {

	Vector3 p1 = body->getPointInWorldSpace(connectionPoint);
	Vector3 p2 = other->getPointInWorldSpace(otherConnectionPoint);

	Vector3 force = p1 - p2;

	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	force.normalise();
	force *= -magnitude;

	body->addForceAtPoint(force, p1);
}

void RotationCW::updateForce(RigidBody* body, real duration)
{
	body->addTorque(Vector3(0, -1, 0)*5);
}

void RotationCCW::updateForce(RigidBody* body, real duration)
{
	body->addTorque(Vector3(0, 1, 0)*5);
}

void Carambolage::updateForce(RigidBody* body, real duration)
{
	if (id == 0)
	{
		body->addTorque(Vector3(0,1,0)*8);
		body->addForce(Vector3(0.5, 0, -1)*10);
	}
	
	if (id == 1)
	{
		body->addTorque(Vector3(0,-1,0)*8);
		body->addForce(Vector3(-0.5, 0, 1)*5);
	}
}



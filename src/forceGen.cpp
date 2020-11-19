#include <typhon/forceGen.h>

using namespace typhon;

void ForceRegistry::updateForces(real duration)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		i->fg->updateForce(i->body, duration);
	}
}

void ForceRegistry::add(RigidBody* body, ForceGenerator* fg)
{
	ForceRegistry::ForceRegistration registration;
	registration.body = body;
	registration.fg = fg;
	registrations.push_back(registration);
}

void Gravity::updateForce(RigidBody* body, real duration)
{
	// Check that we do not have infinite mass
		if (!body->hasFiniteMass()) return;
	// Apply the mass-scaled force to the body
	body->addForce(gravity * body->getMass());
}

void Spring::updateForce(RigidBody* body, real duration)
{
	// Calculate the two ends in world space.
	Vector3 lws = body->getPointInWorldSpace(connectionPoint);
	Vector3 ows = other->getPointInWorldSpace(otherConnectionPoint);
	// Calculate the vector of the spring.
	Vector3 force = lws - ows;
	// Calculate the magnitude of the force.
	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;
	// Calculate the final force and apply it.
	force.normalise();
	force *= -magnitude;
	body->addForceAtPoint(force, lws);
}


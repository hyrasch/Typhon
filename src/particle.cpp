#include <stdexcept>
#include "headers/particle.h"

void Particle::integrate(typhon::real duration) {
	// We don't integrate things with zero mass.
	if (inverseMass <= 0.0f) return;

	if (duration < 0.0) throw std::invalid_argument("Can't integrate on negative duration");

	// Update linear position.
	position.addScaledVector(velocity, duration);

	// Work out the acceleration from the force
	Vector3 resultingAcc = acceleration;
	resultingAcc.addScaledVector(forceAccum, inverseMass);

	// Update linear velocity from the acceleration.
	velocity.addScaledVector(resultingAcc, duration);

	// Impose drag.
	velocity *= real_pow(damping, duration);

	// Clear the forces.
	clearAccumulator();
}



void Particle::setMass(const typhon::real mass) {
	if (mass == 0) throw std::invalid_argument("Must set a non-zero mass");
	Particle::inverseMass = ((typhon::real)1.0) / mass;
}

typhon::real Particle::getMass() const {
	if (inverseMass == 0)
		return REAL_MAX;
	else
		return ((typhon::real)1.0) / inverseMass;
}

void Particle::setInverseMass(const typhon::real inverseMass) {
	Particle::inverseMass = inverseMass;
}

typhon::real Particle::getInverseMass() const {
	return inverseMass;
}

bool Particle::hasFiniteMass() const {
	return inverseMass >= 0.0f;
}

void Particle::setDamping(const typhon::real damping) {
	Particle::damping = damping;
}

typhon::real Particle::getDamping() const {
	return damping;
}

void Particle::setPosition(const Vector3& position) {
	Particle::position = position;
}

void Particle::setPosition(const typhon::real x, const typhon::real y, const typhon::real z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void Particle::getPosition(Vector3* position) const {
	*position = Particle::position;
}

Vector3 Particle::getPosition() const {
	return position;
}

void Particle::setVelocity(const Vector3& velocity) {
	Particle::velocity = velocity;
}

void Particle::setVelocity(const typhon::real x, const typhon::real y, const typhon::real z) {
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;
}

void Particle::getVelocity(Vector3* velocity) const {
	*velocity = Particle::velocity;
}

Vector3 Particle::getVelocity() const {
	return velocity;
}

void Particle::setAcceleration(const Vector3& acceleration) {
	Particle::acceleration = acceleration;
}

void Particle::setAcceleration(const typhon::real x, const typhon::real y, const typhon::real z) {
	acceleration.x = x;
	acceleration.y = y;
	acceleration.z = z;
}

void Particle::getAcceleration(Vector3* acceleration) const {
	*acceleration = Particle::acceleration;
}

Vector3 Particle::getAcceleration() const {
	return acceleration;
}

void Particle::clearAccumulator() {
	forceAccum.clear();
}

void Particle::addForce(const Vector3& force) {
	forceAccum += force;
}
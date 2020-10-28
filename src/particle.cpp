#include <stdexcept>
#include <typhon/particle.h>

using namespace typhon;

void Particle::integrate(real duration) {
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



void Particle::setMass(const real mass) {
	if (mass == 0) throw std::invalid_argument("Must set a non-zero mass");
	Particle::inverseMass = ((real)1.0) / mass;
}

real Particle::getMass() const {
	if (inverseMass == 0)
		return REAL_MAX;
	else
		return ((real)1.0) / inverseMass;
}

void Particle::setInverseMass(const real inverseMass) {
	Particle::inverseMass = inverseMass;
}

real Particle::getInverseMass() const {
	return inverseMass;
}

bool Particle::hasFiniteMass() const {
	return inverseMass >= 0.0f;
}

void Particle::setDamping(const real damping) {
	Particle::damping = damping;
}

real Particle::getDamping() const {
	return damping;
}

void Particle::setPosition(const Vector3& position) {
	Particle::position = position;
}

void Particle::setPosition(const real x, const real y, const real z) {
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

void Particle::setVelocity(const real x, const real y, const real z) {
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

void Particle::setAcceleration(const real x, const real y, const real z) {
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

void Particle::setFlaque() {
	if (position.x <= 10 && position.x >= -10 && position.z <= -30 && position.z >= -50)
	{
		onWater = true;
		onGround = false;
	}
	else
		onWater = false;
}

void Particle::setFlaque(bool flaque) {
	onWater = flaque;
}

bool Particle::getFlaque() {
	return onWater;
}

void Particle::setGround() {
	if (position.x <= 50 && position.x >= -50 && position.z <= 50 && position.z >= -50 && !onWater)
	{
		onGround = true;
		onWater = false;
	}
	else
		onGround = false;
}

void Particle::setGround(bool ground) {
	onGround = ground;
}

bool Particle::getGround() {
	return onGround;
}

void Particle::setToogle(bool toogle) {
	toggle = toogle;
}

bool Particle::getToogle() {
	return toggle;
}

void Particle::setWatToGrnd(bool wattogrnd)
{
	waterToGround = wattogrnd;
	groundToWater = false;
}

bool Particle::getWatToGrnd()
{
	return waterToGround;
}

void Particle::setGrndToWat(bool grndtowat)
{
	groundToWater = grndtowat;
	waterToGround = false;
}

bool Particle::getGrndToWat()
{
	return groundToWater;
}
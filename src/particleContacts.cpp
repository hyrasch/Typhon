#include <typhon/particleContacts.h>

using namespace typhon;

void ParticleContact::resolve(real duration) {
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

real ParticleContact::calculateSeparatingVelocity() const {
	Vector3 relativeVelocity = particle[0]->getVelocity();
	// If there is another particle
	if (particle[1]) relativeVelocity -= particle[1]->getVelocity();

	return relativeVelocity * contactNormal;
}

void ParticleContact::resolveVelocity(real duration) {
	// The velocity at the opposite of the contact
	real separatingVelocity = calculateSeparatingVelocity();
	// If the are separating or not moving, return
	if (separatingVelocity > 0) return;

	// New separating velocity with restitution
	real newSepVelocity = -separatingVelocity * restitution;

	// Checking if both particles are resting
	Vector3 accCausedVelocity = particle[0]->getAcceleration();
	// If there is another particle
	if (particle[1]) accCausedVelocity -= particle[1]->getAcceleration();
	real accCausedSepVelocity = accCausedVelocity *
		contactNormal * duration;

	// If the particles are resting
	if (accCausedSepVelocity < 0) {
		// Remove velocity + checks negative value
		newSepVelocity += restitution * accCausedSepVelocity;
		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	real deltaVelocity = newSepVelocity - separatingVelocity;

	// Applying velocity changes with their inverse mass
	real totalInverseMass = particle[0]->getInverseMass();
	// If it collides with another particle
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	// No effect on infinite masses
	if (totalInverseMass <= 0) return;

	// Impulse resulting
	real impulse = deltaVelocity / totalInverseMass;

	// Vector of impulsion
	Vector3 impulsePerIMass = contactNormal * impulse;

	// Updating velocity
	particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerIMass * particle[0]->getInverseMass());
	// If it collides with another particle, the other one goes in the other direction
	if (particle[1])
		particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerIMass * -particle[1]->getInverseMass());
}

void ParticleContact::resolveInterpenetration(real duration) {
	// If they don't overlay each other
	if (penetration <= 0) return;

	// Applying position changes with their invers mass
	real totalInverseMass = particle[0]->getInverseMass();
	// If it overlays with another particle
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	// Don't deal with infinite mass particles
	if (totalInverseMass <= 0) return;

	// Apply the new position based on the respective inverse mass
	Vector3 movePerIMass = contactNormal * (-penetration / totalInverseMass);

	particle[0]->setPosition(particle[0]->getPosition() +
		movePerIMass * particle[0]->getInverseMass());
	// If it overlays on another particle
	if (particle[1])
		particle[1]->setPosition(particle[1]->getPosition() +
			movePerIMass * particle[1]->getInverseMass());
}

void ParticleContactResolver::setIterations(unsigned iterations) {
	ParticleContactResolver::iterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration) {
	iterationsUsed = 0;

	while (iterationsUsed < iterations) {
		// Holding the min separation velocity of the frame
		real max = REAL_MAX;
		// All the contacts in the frame
		unsigned maxIndex = numContacts;

		for (unsigned i = 0; i < numContacts; i++) {
			real sepVel = contactArray[i].calculateSeparatingVelocity();
			// Updating the min separation velocity (fastest approahing particles)
			if (sepVel < max) {
				max = sepVel;
				maxIndex = i;
			}
		}

		// If we have nothing to resolve
		if (maxIndex == numContacts) break;

		contactArray[maxIndex].resolve(duration);
		iterationsUsed++;
	}
}
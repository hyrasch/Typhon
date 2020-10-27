#include <typhon/particleLinks.h>

using namespace typhon;

real ParticleLink::currentLength() const {
	Vector3 relativePos = particle[0]->getPosition() -
		particle[1]->getPosition();
	return relativePos.magnitude();
}

unsigned ParticleCable::fillContact(ParticleContact* contact, unsigned limit) const {
	// Current length at this frame
	real length = currentLength();

	// If the cable does not overextend, return
	if (length < maxLength) return 0;

	// Otherwise, create a contact
	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];

	// Normal for the contact
	Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
	normal.normalise();
	contact->contactNormal = normal;
	contact->penetration = length - maxLength;
	contact->restitution = restitution;
	return 1;
}

unsigned ParticleRod::fillContact(ParticleContact* contact, unsigned limit) const {
	// Current length in the frame
	real currentLen = currentLength();

	// If the ends are still, return
	if (currentLen == length) return 0;

	// Otherwise, create a contact
	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];

	// Normal for the contact
	Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
	normal.normalise();

	// If the ends are too far
	if (currentLen > length) {
		contact->contactNormal = normal;
		contact->penetration = currentLen - length;
	}
	else { // If the ends are too close
		contact->contactNormal = normal.opposite();
		contact->penetration = length - currentLen;
	}

	// Resting
	contact->restitution = 0;

	return 1;
}

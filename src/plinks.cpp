#include <cyclone/plinks.h>

using namespace cyclone;

real ParticleLink::currentLength() const
{
	Vector3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
	return relativePos.magnitude();
}

unsigned ParticleCable::fillContact(ParticleContact * contact, unsigned limit) const
{

	real length = currentLength();

	if (length < maxLength)
	{
		return 0;
	}

	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];

	Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
	normal.normalise();
	contact->contactNormal = normal;
	contact->penetration = length - maxLength;
	contact->restitution = restitution;
	return 1;
}

unsigned ParticleRod::fillContact(ParticleContact* contact, unsigned limit) const
{

	real currentLen = currentLength();

		if (currentLen == length)
		{
			return 0;
		}

	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];

	Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
	normal.normalise();

	if (currentLen > length) 
	{
		contact->contactNormal = normal;
		contact->penetration = currentLen - length;
	}

	else {
		contact->contactNormal = normal * -1;
		contact->penetration = length - currentLen;
	}

	contact->restitution = 0;
	return 1;
}
#ifndef PLINKS_H
#define PLINKS_H

#include "pcontacts.h"

namespace typhon {

	class ParticleLink
	{
	public:

		Particle* particle[2];
	protected:

		real currentLength() const;
	public:

		virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const = 0;
	};

	class ParticleCable : public ParticleLink
	{
	public:

		real maxLength;

		real restitution;
	public:

		virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const;
	};

	class ParticleRod : public ParticleLink
	{
	public:

		real length;
	public:

		real currentLength() const;

		virtual unsigned fillContact(ParticleContact* contact, unsigned limit) const;
	};
}

#endif PLINKS_H
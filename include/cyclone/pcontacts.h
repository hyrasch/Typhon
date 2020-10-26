#include "particle.h"

namespace cyclone {

	class ParticleContactResolver;

	class ParticleContact
	{

		friend class ParticleContactResolver;

	public:
		Particle* particle[2];

		real restitution;

		Vector3 contactNormal;

		real penetration;
	protected:

		void resolve(real duration);

		real calculateSeparatingVelocity() const;
	private:

		void resolveVelocity(real duration);

		void resolveInterpenetration(real duration);
	};

	class ParticleContactResolver
	{
	protected:

		unsigned iterations;

		unsigned iterationsUsed;
	public:

		ParticleContactResolver(unsigned iterations);

		void setIterations(unsigned iterations);

		void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
	};

}
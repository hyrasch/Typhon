#ifndef TYPHON_PARTICLE_H
#define TYPHON_PARTICLE_H

#include "core.h"

class Particle {
protected:
	typhon::real inverseMass;
	typhon::real damping;

	Vector3 position;
	Vector3 velocity;

	Vector3 forceAccum;
	Vector3 acceleration;

public:
	void integrate(typhon::real duration);

	void setMass(const typhon::real mass);
	typhon::real getMass() const;
	void setInverseMass(const typhon::real inverseMass);
	typhon::real getInverseMass() const;

	bool hasFiniteMass() const;

	void setDamping(const typhon::real damping);
	typhon::real getDamping() const;

	void setPosition(const Vector3& position);
	void setPosition(const typhon::real x, const typhon::real y, const typhon::real z);

	void getPosition(Vector3* position) const;
	Vector3 getPosition() const;

	void setVelocity(const Vector3& velocity);
	void setVelocity(const typhon::real x, const typhon::real y, const typhon::real z);

	void getVelocity(Vector3* velocity) const;
	Vector3 getVelocity() const;

	void setAcceleration(const Vector3& acceleration);
	void setAcceleration(const typhon::real x, const typhon::real y, const typhon::real z);
	void getAcceleration(Vector3* acceleration) const;
	Vector3 getAcceleration() const;

	void clearAccumulator();
	void addForce(const Vector3& force);
};

#endif  TYPHON_PARTICLE_H
#include "typhon/collision.h"
#include "typhon/intersection.h"

using namespace typhon;

BoundingSphere::BoundingSphere()
{
	BoundingSphere::centre = Vector3(0,0,0);
	BoundingSphere::radius = 1;
}

BoundingSphere::BoundingSphere(const Vector3& centre, real radius)
{
	BoundingSphere::centre = centre;
	BoundingSphere::radius = radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere& one,
	const BoundingSphere& two)
{
	Vector3 centreOffset = two.centre - one.centre;
	real distance = centreOffset.squareMagnitude();
	real radiusDiff = two.radius - one.radius;

	if (radiusDiff * radiusDiff >= distance)
	{
		if (one.radius > two.radius)
		{
			centre = one.centre;
			radius = one.radius;
		}
		else
		{
			centre = two.centre;
			radius = two.radius;
		}
	}

	else
	{
		distance = real_sqrt(distance);
		radius = (distance + one.radius + two.radius) * ((real)0.5);

		centre = one.centre;
		if (distance > 0)
		{
			centre += centreOffset * ((radius - one.radius) / distance);
		}
	}

}

bool BoundingSphere::overlaps(const BoundingSphere* other) const
{
	real distanceSquared = (centre - other->centre).squareMagnitude();
	return distanceSquared < (radius + other->radius)* (radius + other->radius);
}

bool BoundingSphere::overlaps(const BoundingSphere* first, const BoundingSphere* sec) const
{
	real distanceSquared = (first->centre - sec->centre).squareMagnitude();
	return distanceSquared < (first->radius + sec->radius)* (first->radius + sec->radius);
}

real BoundingSphere::getGrowth(const BoundingSphere& other) const
{
	BoundingSphere newSphere(*this, other);

	return newSphere.radius * newSphere.radius - radius * radius;
}

unsigned CollisionDetector::boxXhalfSpace(const ColBox& box, const ColPlane& plane, CollisionData* data) {
	// S'il y a bien des contacts
	if (data->contactsLeft <= 0) return 0;

	// S'il y a bien intersection
	if (!IntersectionTest::boxXhalfSpace(box, plane)) return 0;

	// Trouver le point d'intersection
	static real vertices[8][3] = {
		{1,1,1}, {-1,1,1}, {1,-1,1}, {-1,-1,1},
		{1,1,-1}, {-1,1,-1}, {1,-1,-1}, {-1,-1,-1} };

	Contact* contact = data->contacts;

	unsigned used = 0;
	for (unsigned i = 0; i < 8; i++) {
		// Positions de chaque sommet
		Vector3 vertexPos(vertices[i][0], vertices[i][1], vertices[i][2]);
		// Mise à l'échelle
		vertexPos.componentProductUpdate(box.halfSize);
		// Transformée dans le monde
		vertexPos = box.getTransform().transform(vertexPos);

		// Distance p/r au plan
		real dist = vertexPos * plane.normal;

		// Si le sommet est dans le plan
		if (dist <= plane.offset) {
			// Génération du contact
			contact->contactNormal = plane.normal;
			// Point de contact à mis-chemin entre le sommet et le plan
			contact->contactPoint = vertexPos + plane.normal * (dist - plane.offset);
			contact->penetration = plane.offset - dist;

			contact->setBodyData(box.body, plane.body , data->friction, data->restitution);

			// Prochain contact
			contact++;
			used++;
			if (used == data->contactsLeft) return used;
		}
	}

	data->addContacts(used);
	return used;
}
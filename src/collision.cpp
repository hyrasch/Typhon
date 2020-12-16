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

unsigned CollisionDetector::sphereXsphere(const ColSphere& first, const ColSphere& sec, CollisionData* data) {
	// S'il y a bien des contacts
	if (data->contactsLeft <= 0) return 0;

	// Positions
	Vector3 posFirst = first.getAxis(3);
	Vector3 posSec = sec.getAxis(3);

	// Vecteur séparant les 2 sphères
	Vector3 between = posFirst - posSec;
	real magn = between.magnitude();

	// Si la séparation des 2 sphères est trop petite ou trop grande
	if (magn <= 0.0f || magn >= first.radius + sec.radius) return 0;

	// Génération de la normale
	Vector3 normal = between * (((real)1.0) / magn);

	// Génération du contact
	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->contactPoint = between * (real)0.5 + posFirst;
	contact->penetration = first.radius + sec.radius - magn;
	contact->setBodyData(first.body, sec.body, data->friction, data->restitution);

	// Mise à jour du pointeur
	data->addContacts(1);
	return 1;
}

unsigned CollisionDetector::sphereXhalfSpace(const ColSphere& sphere, const ColPlane& plane, CollisionData* data) {
	// S'il y a bien des contacts
	if (data->contactsLeft <= 0) return 0;

	// Position de la sphère
	Vector3 position = sphere.getAxis(3);

	// Distance de la sphère p/r au plan
	real dist = plane.normal * position - sphere.radius;

	// Si la sphère ne touche pas le plan
	if (dist >= plane.offset) return 0;

	// Génération du contact
	Contact* contact = data->contacts;
	contact->contactNormal = plane.normal;
	// Point de contact p/r à l'intersection rayon-plan
	contact->contactPoint = position - plane.normal * (dist - sphere.radius);
	contact->penetration = -dist;
	contact->setBodyData(sphere.body, plane.body , data->friction, data->restitution);

	// Mise à jour du pointeur
	data->addContacts(1);
	return 1;
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
			std::cout << "On est dans le plan" << std::endl;
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
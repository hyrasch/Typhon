#include "typhon/intersection.h"

using namespace typhon;

real IntersectionTest::transformToAxis(const ColBox& box, const Vector3& axis) {
	return box.halfSize.x * real_abs(axis * box.getAxis(0)) +
		box.halfSize.y * real_abs(axis * box.getAxis(1)) +
		box.halfSize.z * real_abs(axis * box.getAxis(2));
}

bool IntersectionTest::boxXhalfSpace(const ColBox& box, const ColPlane& plane) {
	// Rayon de la boîte p/r à la normale du plan
	real projected = transformToAxis(box, plane.normal);

	// Distance de la boîte p/r au plan
	real dist = plane.normal * box.getAxis(3) - projected;

	// Si la boîte traverse d'une certaine manière le plan
	return dist <= plane.offset;
}
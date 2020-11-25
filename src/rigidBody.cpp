#include <typhon/typhon.h>
#include <assert.h>

using namespace typhon;

// Transformée 
inline void _calculateTransformMatrix(Matrix4& transformMatrix, const Vector3& position, const Quaternion& orientation) {
	transformMatrix.val[0] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
	transformMatrix.val[1] = 2 * orientation.i * orientation.j - 2 * orientation.r * orientation.k;
	transformMatrix.val[2] = 2 * orientation.i * orientation.k + 2 * orientation.r * orientation.j;
	transformMatrix.val[3] = position.x;

	transformMatrix.val[4] = 2 * orientation.i * orientation.j + 2 * orientation.r * orientation.k;
	transformMatrix.val[5] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k;
	transformMatrix.val[6] = 2 * orientation.j * orientation.k - 2 * orientation.r * orientation.i;
	transformMatrix.val[7] = position.y;

	transformMatrix.val[8] = 2 * orientation.i * orientation.k - 2 * orientation.r * orientation.j;
	transformMatrix.val[9] = 2 * orientation.j * orientation.k + 2 * orientation.r * orientation.i;
	transformMatrix.val[10] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j;
	transformMatrix.val[11] = position.z;
}

// Transformée du tenseur d'inertie (body space ==> world space)
inline void _transformInertiaTensor(Matrix3& iitWorld, const Quaternion& q, const Matrix3& iitBody, const Matrix4& rotmat) {
	real t4 = rotmat.val[0] * iitBody.val[0] + rotmat.val[1] * iitBody.val[3] + rotmat.val[2] * iitBody.val[6];
	real t9 = rotmat.val[0] * iitBody.val[1] + rotmat.val[1] * iitBody.val[4] + rotmat.val[2] * iitBody.val[7];
	real t14 = rotmat.val[0] * iitBody.val[2] + rotmat.val[1] * iitBody.val[5] + rotmat.val[2] * iitBody.val[8];
	real t28 = rotmat.val[4] * iitBody.val[0] + rotmat.val[5] * iitBody.val[3] + rotmat.val[6] * iitBody.val[6];
	real t33 = rotmat.val[4] * iitBody.val[1] + rotmat.val[5] * iitBody.val[4] + rotmat.val[6] * iitBody.val[7];
	real t38 = rotmat.val[4] * iitBody.val[2] + rotmat.val[5] * iitBody.val[5] + rotmat.val[6] * iitBody.val[8];
	real t52 = rotmat.val[8] * iitBody.val[0] + rotmat.val[9] * iitBody.val[3] + rotmat.val[10] * iitBody.val[6];
	real t57 = rotmat.val[8] * iitBody.val[1] + rotmat.val[9] * iitBody.val[4] + rotmat.val[10] * iitBody.val[7];
	real t62 = rotmat.val[8] * iitBody.val[2] + rotmat.val[9] * iitBody.val[5] + rotmat.val[10] * iitBody.val[8];

	iitWorld.val[0] = t4 * rotmat.val[0] + t9 * rotmat.val[1] + t14 * rotmat.val[2];
	iitWorld.val[1] = t4 * rotmat.val[4] + t9 * rotmat.val[5] + t14 * rotmat.val[6];
	iitWorld.val[2] = t4 * rotmat.val[8] + t9 * rotmat.val[9] + t14 * rotmat.val[10];
	iitWorld.val[3] = t28 * rotmat.val[0] + t33 * rotmat.val[1] + t38 * rotmat.val[2];
	iitWorld.val[4] = t28 * rotmat.val[4] + t33 * rotmat.val[5] + t38 * rotmat.val[6];
	iitWorld.val[5] = t28 * rotmat.val[8] + t33 * rotmat.val[9] + t38 * rotmat.val[10];
	iitWorld.val[6] = t52 * rotmat.val[0] + t57 * rotmat.val[1] + t62 * rotmat.val[2];
	iitWorld.val[7] = t52 * rotmat.val[4] + t57 * rotmat.val[5] + t62 * rotmat.val[6];
	iitWorld.val[8] = t52 * rotmat.val[8] + t57 * rotmat.val[9] + t62 * rotmat.val[10];
}

// Récupère le tenseur d'intertie de l'objet dans le world space
void RigidBody::calculateInertiaTensorWS() {
	// Le quaternion doit représenter uniquement une orientation
	// Il doit être normalisé régulièrement afin d'éviter les erreurs d'approximation
	orientation.normalise();

	// Calcule la matrice de transformation (BodySpace)
	_calculateTransformMatrix(transformMatrix, position, orientation);

	// Calcule le tenseur d'inertie (WorldSpace)
	// N'ayant besoin que de l'orientation de l'objet, on récupère une matrice 3x3
	_transformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
}

void RigidBody::integrate(real duration) {
	// Calculer l'accéleration linéaire à partir de forceAccum
	lastFrameAcceleration = acceleration;
	lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);

	// Calculer l'accéleration angulaire à partir de torqueAccum
	Vector3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);


	// MAJ des vélocités
	velocity.addScaledVector(lastFrameAcceleration, duration);
	rotation.addScaledVector(angularAcceleration, duration);


	// Ajout des ammortissements
	velocity *= real_pow(linearDamping, duration);
	rotation *= real_pow(angularDamping, duration);

	// MAJ de l'objet dans l'espace
	position.addScaledVector(velocity, duration);
	orientation.addScaledVector(rotation, duration);


	// Calculer les "DerivedData" (Matrice de transformation et tenseur d'inertie)
	calculateInertiaTensorWS();

	// Nettoyage
	clearAccumulators();
}

void RigidBody::addRotation(const Vector3& deltaRotation) {
	rotation += deltaRotation;
}

void RigidBody::clearAccumulators() {
	forceAccum.clear();
	torqueAccum.clear();
}

// Ajoute une force au centre de masse du RigidBody
void RigidBody::addForce(const Vector3& force) {
	forceAccum += force;
}

// Applique une force en un point donné du RigidBody (fixe)
// Force(WorldSpace) | Point(BodySpace)
// Idéal pour des forces attachées au RigidBody
void RigidBody::addForceAtBodyPoint(const Vector3& force, const Vector3& point) {
	Vector3 pt = getPointInWorldSpace(point);
	addForceAtPoint(force, pt);
}

// Applique une force en un point du RigidBody
// Force(WorldSpace) | Point(WorldSpace)
void RigidBody::addForceAtPoint(const Vector3& force, const Vector3& point) {
	// On se positionne p/r au centre de masse (BodySpace)
	Vector3 pt = point - position;

	forceAccum += force;
	torqueAccum += pt % force;

}

void RigidBody::addTorque(const Vector3& torque) {
	torqueAccum += torque;
}

//Getters & Setters
#pragma region Get & Set

void RigidBody::setMass(const real mass) {
	assert(mass != 0);
	RigidBody::inverseMass = ((real)1.0) / mass;
}

real RigidBody::getMass() const {
	if (inverseMass == 0) {
		return REAL_MAX;
	}
	else {
		return ((real)1.0) / inverseMass;
	}
}

void RigidBody::setInverseMass(const real inverseMass) {
	RigidBody::inverseMass = inverseMass;
}

real RigidBody::getInverseMass() const {
	return inverseMass;
}

bool RigidBody::hasFiniteMass() const {
	return inverseMass >= 0.0f;
}

// Initalise le tenseur d'inertie inverse
void RigidBody::setInertiaTensor(const Matrix3& inertiaTensor) {
	inverseInertiaTensor.setInverse(inertiaTensor);
}

void RigidBody::getInertiaTensor(Matrix3* inertiaTensor) const {
	inertiaTensor->setInverse(inverseInertiaTensor);
}

Matrix3 RigidBody::getInertiaTensor() const {
	Matrix3 it;
	getInertiaTensor(&it);
	return it;
}

void RigidBody::getInertiaTensorWorld(Matrix3* inertiaTensor) const {
	inertiaTensor->setInverse(inverseInertiaTensorWorld);
}

Matrix3 RigidBody::getInertiaTensorWorld() const {
	Matrix3 it;
	getInertiaTensorWorld(&it);
	return it;
}

void RigidBody::setInverseInertiaTensor(const Matrix3& inverseInertiaTensor) {
	RigidBody::inverseInertiaTensor = inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensor(Matrix3* inverseInertiaTensor) const {
	*inverseInertiaTensor = RigidBody::inverseInertiaTensor;
}

Matrix3 RigidBody::getInverseInertiaTensor() const {
	return inverseInertiaTensor;
}

void RigidBody::getInverseInertiaTensorWorld(Matrix3* inverseInertiaTensor) const {
	*inverseInertiaTensor = inverseInertiaTensorWorld;
}

Matrix3 RigidBody::getInverseInertiaTensorWorld() const {
	return inverseInertiaTensorWorld;
}

void RigidBody::setDamping(const real linearDamping, const real angularDamping) {
	RigidBody::linearDamping = linearDamping;
	RigidBody::angularDamping = angularDamping;
}

void RigidBody::setLinearDamping(const real linearDamping) {
	RigidBody::linearDamping = linearDamping;
}

real RigidBody::getLinearDamping() const {
	return linearDamping;
}

void RigidBody::setAngularDamping(const real angularDamping) {
	RigidBody::angularDamping = angularDamping;
}

real RigidBody::getAngularDamping() const {
	return angularDamping;
}

void RigidBody::setPosition(const Vector3& position) {
	RigidBody::position = position;
}

void RigidBody::setPosition(const real x, const real y, const real z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void RigidBody::getPosition(Vector3* position) const {
	*position = RigidBody::position;
}

Vector3 RigidBody::getPosition() const {
	return position;
}

void RigidBody::setOrientation(const Quaternion& orientation) {
	RigidBody::orientation = orientation;
	RigidBody::orientation.normalise();
}

void RigidBody::setOrientation(const real r, const real i, const real j, const real k) {
	orientation.r = r;
	orientation.i = i;
	orientation.j = j;
	orientation.k = k;
	orientation.normalise();
}

void RigidBody::getOrientation(Quaternion* orientation) const {
	*orientation = RigidBody::orientation;
}

Quaternion RigidBody::getOrientation() const {
	return orientation;
}

void RigidBody::getOrientation(Matrix3* matrix) const {
	getOrientation(matrix->val);
}

void RigidBody::getOrientation(real matrix[9]) const {
	matrix[0] = transformMatrix.val[0];
	matrix[1] = transformMatrix.val[1];
	matrix[2] = transformMatrix.val[2];

	matrix[3] = transformMatrix.val[4];
	matrix[4] = transformMatrix.val[5];
	matrix[5] = transformMatrix.val[6];

	matrix[6] = transformMatrix.val[8];
	matrix[7] = transformMatrix.val[9];
	matrix[8] = transformMatrix.val[10];
}

void RigidBody::getTransform(Matrix4* transform) const {
	memcpy(transform, &transformMatrix.val, sizeof(Matrix4));
}

void RigidBody::getTransform(real matrix[16]) const {
	memcpy(matrix, transformMatrix.val, sizeof(real) * 12);
	matrix[12] = matrix[13] = matrix[14] = 0;
	matrix[15] = 1;
}

void RigidBody::getGLTransform(float matrix[16]) const {
	matrix[0] = (float)transformMatrix.val[0];
	matrix[1] = (float)transformMatrix.val[4];
	matrix[2] = (float)transformMatrix.val[8];
	matrix[3] = 0;

	matrix[4] = (float)transformMatrix.val[1];
	matrix[5] = (float)transformMatrix.val[5];
	matrix[6] = (float)transformMatrix.val[9];
	matrix[7] = 0;

	matrix[8] = (float)transformMatrix.val[2];
	matrix[9] = (float)transformMatrix.val[6];
	matrix[10] = (float)transformMatrix.val[10];
	matrix[11] = 0;

	matrix[12] = (float)transformMatrix.val[3];
	matrix[13] = (float)transformMatrix.val[7];
	matrix[14] = (float)transformMatrix.val[11];
	matrix[15] = 1;
}

Matrix4 RigidBody::getTransform() const {
	return transformMatrix;
}


Vector3 RigidBody::getPointInLocalSpace(const Vector3& point) const {
	return transformMatrix.transformInverse(point);
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3& point) const {
	return transformMatrix.transform(point);
}

Vector3 RigidBody::getDirectionInLocalSpace(const Vector3& direction) const {
	return transformMatrix.transformInverseDirection(direction);
}

Vector3 RigidBody::getDirectionInWorldSpace(const Vector3& direction) const {
	return transformMatrix.transformDirection(direction);
}


void RigidBody::setVelocity(const Vector3& velocity) {
	RigidBody::velocity = velocity;
}

void RigidBody::setVelocity(const real x, const real y, const real z) {
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;
}

void RigidBody::getVelocity(Vector3* velocity) const {
	*velocity = RigidBody::velocity;
}

Vector3 RigidBody::getVelocity() const {
	return velocity;
}

void RigidBody::addVelocity(const Vector3& deltaVelocity) {
	velocity += deltaVelocity;
}

void RigidBody::setRotation(const Vector3& rotation) {
	RigidBody::rotation = rotation;
}

void RigidBody::setRotation(const real x, const real y, const real z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void RigidBody::getRotation(Vector3* rotation) const {
	*rotation = RigidBody::rotation;
}

Vector3 RigidBody::getRotation() const {
	return rotation;
}

void RigidBody::getLastFrameAcceleration(Vector3* acceleration) const {
	*acceleration = lastFrameAcceleration;
}

Vector3 RigidBody::getLastFrameAcceleration() const {
	return lastFrameAcceleration;
}

void RigidBody::setAcceleration(const Vector3& acceleration) {
	RigidBody::acceleration = acceleration;
}

void RigidBody::setAcceleration(const real x, const real y, const real z) {
	acceleration.x = x;
	acceleration.y = y;
	acceleration.z = z;
}

void RigidBody::getAcceleration(Vector3* acceleration) const {
	*acceleration = RigidBody::acceleration;
}

Vector3 RigidBody::getAcceleration() const {
	return acceleration;
}

#pragma endregion
#ifndef TYPHON_CORE_H
#define TYPHON_CORE_H

#include <math.h>
#include "precision.h"

namespace typhon {

	class Vector3 {
	public:
		real x;
		real y;
		real z;

	private:
		real pad;

	public:
		// Default constructor
		Vector3() : x(0), y(0), z(0) {}
		// Overloading
		Vector3(const real x, const real y, const real z)
			: x(x), y(y), z(z) {}

		const static Vector3 GRAVITY;
		const static Vector3 HIGH_GRAVITY;
		const static Vector3 UP;
		const static Vector3 RIGHT;
		const static Vector3 OUT_OF_SCREEN;
		const static Vector3 X;
		const static Vector3 Y;
		const static Vector3 Z;

		real operator[](unsigned i) const {
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		real& operator[](unsigned i) {
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		// Vector addition
		void operator+=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		Vector3 operator+(const Vector3& v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		// Substraction
		void operator-=(const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		Vector3 operator-(const Vector3& v) const {
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		// Multiplication
		void operator*=(const real value) {
			x *= value;
			y *= value;
			z *= value;
		}

		Vector3 operator*(const real value) const {
			return Vector3(x * value, y * value, z * value);
		}

		// Component product
		Vector3 componentProduct(const Vector3& vector) const {
			return Vector3(x * vector.x, y * vector.y, z * vector.z);
		}

		void componentProductUpdate(const Vector3& vector) {
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
		}

		// Vector product
		Vector3 vectorProduct(const Vector3& vector) const {
			return Vector3(y * vector.z - z * vector.y,
				z * vector.x - x * vector.z,
				x * vector.y - y * vector.x);
		}

		void operator %=(const Vector3& vector) {
			*this = vectorProduct(vector);
		}

		Vector3 operator%(const Vector3& vector) const {
			return Vector3(y * vector.z - z * vector.y,
				z * vector.x - x * vector.z,
				x * vector.y - y * vector.x);
		}

		// Scalar product
		real scalarProduct(const Vector3& vector) const {
			return x * vector.x + y * vector.y + z * vector.z;
		}

		real operator *(const Vector3& vector) const {
			return x * vector.x + y * vector.y + z * vector.z;
		}

		// Vector addition with scale
		void addScaledVector(const Vector3& vector, real scale) {
			x += vector.x * scale;
			y += vector.y * scale;
			z += vector.z * scale;
		}

		// Vector magnitude
		real magnitude() const {
			return real_sqrt(x * x + y * y + z * z);
		}

		// Vectorsquared magnitude
		real squareMagnitude() const {

			return x * x + y * y + z * z;
		}

		// Vector vormalization
		void normalise() {
			real l = magnitude();
			if (l > 0)
				(*this) *= ((real)1) / l;
		}

		// Returns a normalized vector version
		Vector3 unit() const {
			Vector3 result = *this;
			result.normalise();
			return result;
		}

		// Overloading component - component verification
		bool operator==(const Vector3& other) const {
			return x == other.x &&
				y == other.y &&
				z == other.z;
		}

		bool operator!=(const Vector3& other) const {
			return !(*this == other);
		}

		bool operator<(const Vector3& other) const {
			return x < other.x&& y < other.y&& z < other.z;
		}

		bool operator>(const Vector3& other) const {
			return x > other.x && y > other.y && z > other.z;
		}

		bool operator<=(const Vector3& other) const {
			return x <= other.x && y <= other.y && z <= other.z;
		}

		bool operator>=(const Vector3& other) const {
			return x >= other.x && y >= other.y && z >= other.z;
		}

		// Zeros
		void clear() {
			x = y = z = 0;
		}

		// Vector inversion
		void invert() {
			x = -x;
			y = -y;
			z = -z;
		}

		// Returns an inverted vector
		Vector3 opposite() const {
			Vector3 res = *this;
			res.invert();
			return res;
		}
	};

	class Quaternion
	{
	public:
		union {
			struct {

				//Partie réelle
				real r;

				//Partie imaginaire
				real i;
				real j;
				real k;
			};

			//Stockage des valeurs;
			real val[4];
		};

		//Constructeur vide
		Quaternion() : r(1), i(0), j(0), k(0) {}

		//Constructeur avec arguments
		Quaternion(const real r, const real i, const real j, const real k) : r(r), i(i), j(j), k(k) {}

		//Normalise le quaternion
		//A utiliser fréquemment pour que le quaternion répresente une rotation à chaque instant (norme == 1)
		void normalise()
		{
			real d = r * r + i * i + j * j + k * k;

			//Si la norme est nulle -> partie réelle = 1
			if (d == 0) {
				r = 1;
				return;
			}
			d = ((real)1.0) / real_sqrt(d);
			r *= d;
			i *= d;
			j *= d;
			k *= d;
		}

		//Multiplication du quaternion actuel avec un autre quaternion
		void operator *=(const Quaternion& multiplier)
		{
			Quaternion q = *this;
			r = q.r * multiplier.r - q.i * multiplier.i -
				q.j * multiplier.j - q.k * multiplier.k;
			i = q.r * multiplier.i + q.i * multiplier.r +
				q.j * multiplier.k - q.k * multiplier.j;
				j = q.r * multiplier.j + q.j * multiplier.r +
				q.k * multiplier.i - q.i * multiplier.k;
			k = q.r * multiplier.k + q.k * multiplier.r +
				q.i * multiplier.j - q.j * multiplier.i;
		}

		//Rotation du quaternion autour d'un vecteur
		void rotateByVector(const Vector3& vector)
		{
			Quaternion q(0, vector.x, vector.y, vector.z);
			(*this) *= q;
		}

		//Met a jour l'orientation du quaternion à l'aide de la velocité angulaire
		void addScaledVector(const Vector3& vector, real scale)
		{
			Quaternion q(0,
				vector.x * scale,
				vector.y * scale,
				vector.z * scale);
			q *= *this;
			r += q.r * ((real)0.5);
			i += q.i * ((real)0.5);
			j += q.j * ((real)0.5);
			k += q.k * ((real)0.5);
		}
	};

	class Matrix3
	{
	public:

		//Stockage des valeurs
		real val[9];

		//Constructeur vide
		Matrix3() 
		{
			val[0] = val[1] = val[2] = 
			val[3] = val[4] = val[5] = 
			val[6] = val[7] = val[8] = 0;
		}

		//Constructeur avec arguments
		Matrix3(real m0, real m1, real m2, real m3, real m4, real m5,
			real m6, real m7, real m8)
		{
			val[0] = m0; val[1] = m1; val[2] = m2;
			val[3] = m3; val[4] = m4; val[5] = m5;
			val[6] = m6; val[7] = m7; val[8] = m8;
		}

		Vector3 operator*(const Vector3& vector) const
		{
			return Vector3(
				vector.x * val[0] + vector.y * val[1] + vector.z * val[2],
				vector.x * val[3] + vector.y * val[4] + vector.z * val[5],
				vector.x * val[6] + vector.y * val[7] + vector.z * val[8]
			);
		}

		//Transformé du vecteur3 par la matrice actuelle
		Vector3 transform(const Vector3& vector) const
		{
			return (*this) * vector;
		}

		//Multiplication d'une matrice 3x3 par la matrice actuelle (3x3 aussi)
		Matrix3 operator*(const Matrix3& other) const
		{
			//On utilise le constructeur de Matrix3 pour renvoyer le résultat de la mutiplication
			return Matrix3(
				val[0] * other.val[0] + val[1] * other.val[3] + val[2] * other.val[6],
				val[0] * other.val[1] + val[1] * other.val[4] + val[2] * other.val[7],
				val[0] * other.val[2] + val[1] * other.val[5] + val[2] * other.val[8],

				val[3] * other.val[0] + val[4] * other.val[3] + val[5] * other.val[6],
				val[3] * other.val[1] + val[4] * other.val[4] + val[5] * other.val[7],
				val[3] * other.val[2] + val[4] * other.val[5] + val[5] * other.val[8],

				val[6] * other.val[0] + val[7] * other.val[3] + val[8] * other.val[6],
				val[6] * other.val[1] + val[7] * other.val[4] + val[8] * other.val[7],
				val[6] * other.val[2] + val[7] * other.val[5] + val[8] * other.val[8]
			);
		}

		//Calcul determinant
		real getDeterminant() const
		{
			return val[0] * val[4] * val[8] -
				val[0] * val[5] * val[7] -
				val[1] * val[3] * val[8] +
				val[2] * val[3] * val[7] +
				val[1] * val[6] * val[5] -
				val[2] * val[6] * val[4];
		}

		void setInverse(const Matrix3& matrix)
		{
			//Calcul determinant
			real t4 = matrix.val[0] * matrix.val[4];
			real t6 = matrix.val[0] * matrix.val[5];
			real t8 = matrix.val[1] * matrix.val[3];
			real t10 = matrix.val[2] * matrix.val[3];
			real t12 = matrix.val[1] * matrix.val[6];
			real t14 = matrix.val[2] * matrix.val[6];
			real t16 = (t4 * matrix.val[8] - t6 * matrix.val[7] - t8 * matrix.val[8] + t10 * matrix.val[7] + t12 * matrix.val[5] - t14 * matrix.val[4]);

			//t16 = getDeterminant(); A VERIFIEEEEEEEEEEEEEEEEEEEEEEEEEER

			//Critère d'inversibilité ( det(matrix) != 0 )
			if (t16 == (real)0.0f) return; 

			//Calcul matrice inverse
			real t17 = 1 / t16;
			val[0] = (matrix.val[4] * matrix.val[8] - matrix.val[5] * matrix.val[7]) * t17;
			val[1] = -(matrix.val[1] * matrix.val[8] - matrix.val[2] * matrix.val[7]) * t17;
			val[2] = (matrix.val[1] * matrix.val[5] - matrix.val[2] * matrix.val[4]) * t17;
			val[3] = -(matrix.val[3] * matrix.val[8] - matrix.val[5] * matrix.val[6]) * t17;
			val[4] = (matrix.val[0] * matrix.val[8] - t14) * t17;
			val[5] = -(t6 - t10) * t17;
			val[6] = (matrix.val[3] * matrix.val[7] - matrix.val[4] * matrix.val[6]) * t17;
			val[7] = -(matrix.val[0] * matrix.val[7] - t12) * t17;
			val[8] = (t4 - t8) * t17;
		}

		void setTranspose(const Matrix3& matrix)
		{
			val[0] = matrix.val[0];
			val[1] = matrix.val[3];
			val[2] = matrix.val[6];
			val[3] = matrix.val[1];
			val[4] = matrix.val[4];
			val[5] = matrix.val[7];
			val[6] = matrix.val[2];
			val[7] = matrix.val[5];
			val[8] = matrix.val[8];
		}
		
		//Créée une nouvelle matrice étant l'inverse de la matrice actuelle
		Matrix3 inverse() const
		{
			Matrix3 result;
			result.setInverse(*this);
			return result;
		}

		//Inverse la matrice actuelle
		void invert()
		{
			setInverse(*this);
		}

		//Créée une nouvelle matrice étant la transposée de la matrice actuelle
		Matrix3 transpose() const
		{
			Matrix3 result;
			result.setTranspose(*this);
			return result;
		}

		//Transpose la matrice actuelle
		void transp()
		{
			setTranspose(*this);
		}

		//Définie la matrice actuelle comme matrice de rotation du quaternion
		void setOrientation(const Quaternion& quaternion)
		{
			val[0] = 1 - (2 * quaternion.j * quaternion.j + 2 * quaternion.k * quaternion.k);
			val[1] = 2 * quaternion.i * quaternion.j + 2 * quaternion.k * quaternion.r;
			val[2] = 2 * quaternion.i * quaternion.k - 2 * quaternion.j * quaternion.r;
			val[3] = 2 * quaternion.i * quaternion.j - 2 * quaternion.k * quaternion.r;
			val[4] = 1 - (2 * quaternion.i * quaternion.i + 2 * quaternion.k * quaternion.k);
			val[5] = 2 * quaternion.j * quaternion.k + 2 * quaternion.i * quaternion.r;
			val[6] = 2 * quaternion.i * quaternion.k + 2 * quaternion.j * quaternion.r;
			val[7] = 2 * quaternion.j * quaternion.k - 2 * quaternion.i * quaternion.r;
			val[8] = 1 - (2 * quaternion.i * quaternion.i + 2 * quaternion.j * quaternion.j);
		}
	};

	class Matrix4
	{
	public:

		//Stockage des valeurs
		real val[12];

		//Constructeur vide
		Matrix4()
		{
			val[1] = val[2] = val[3] =
			val[4] = val[6] = val[7] =
			val[8] = val[9] = val[11] = 0;

			val[0] = val[5] = val[10] = 1;
		}

		Vector3 transform(const Vector3& vector) const
		{
			return (*this) * vector;
		}

		//Transformé d'un vecteur par la matrice actuelle
		Vector3 operator*(const Vector3& vector) const
		{
			return Vector3(
				vector.x * val[0] +
				vector.y * val[1] +
				vector.z * val[2] + val[3],

				vector.x * val[4] +
				vector.y * val[5] +
				vector.z * val[6] + val[7],

				vector.x * val[8] +
				vector.y * val[9] +
				vector.z * val[10] + val[11]
			);
		}

		//Multiplication d'une matrice 4x4 par la matrice actuelle (4x4 aussi)
		Matrix4 operator*(const Matrix4& other) const
		{
			Matrix4 result;
			result.val[0] = (other.val[0] * val[0]) + (other.val[4] * val[1]) +
				(other.val[8] * val[2]);
			result.val[4] = (other.val[0] * val[4]) + (other.val[4] * val[5]) +
				(other.val[8] * val[6]);
			result.val[8] = (other.val[0] * val[8]) + (other.val[4] * val[9]) +
				(other.val[8] * val[10]);
			result.val[1] = (other.val[1] * val[0]) + (other.val[5] * val[1]) +
				(other.val[9] * val[2]);
			result.val[5] = (other.val[1] * val[4]) + (other.val[5] * val[5]) +
				(other.val[9] * val[6]);
			result.val[9] = (other.val[1] * val[8]) + (other.val[5] * val[9]) +
				(other.val[9] * val[10]);
			result.val[2] = (other.val[2] * val[0]) + (other.val[6] * val[1]) +
				(other.val[10] * val[2]);
			result.val[6] = (other.val[2] * val[4]) + (other.val[6] * val[5]) +
				(other.val[10] * val[6]);
			result.val[10] = (other.val[2] * val[8]) + (other.val[6] * val[9]) +
				(other.val[10] * val[10]);
			result.val[3] = (other.val[3] * val[0]) + (other.val[7] * val[1]) +
				(other.val[11] * val[2]) + val[3];
			result.val[7] = (other.val[3] * val[4]) + (other.val[7] * val[5]) +
				(other.val[11] * val[6]) + val[7];
			result.val[11] = (other.val[3] * val[8]) + (other.val[7] * val[9]) +
				(other.val[11] * val[10]) + val[11];
			return result;
		}

		//Calcul determinant
		real getDeterminant() const
		{
			return val[8] * val[5] * val[2] +
				val[4] * val[9] * val[2] +
				val[8] * val[1] * val[6] -
				val[0] * val[9] * val[6] -
				val[4] * val[1] * val[10] +
				val[0] * val[5] * val[10];
		}

		void setInverse(const Matrix4& matrix)
		{
			//Critère d'inversibilité ( det(matrix) != 0 )
			real det = getDeterminant(); 
			if (det == 0) return;
			det = ((real)1.0) / det;

			val[0] = (-matrix.val[9] * matrix.val[6] + matrix.val[5] * matrix.val[10]) * det;
			val[4] = (matrix.val[8] * matrix.val[6] - matrix.val[4] * matrix.val[10]) * det;
			val[8] = (-matrix.val[8] * matrix.val[5] + matrix.val[4] * matrix.val[9] * matrix.val[15]) * det;
			val[1] = (matrix.val[9] * matrix.val[2] - matrix.val[1] * matrix.val[10]) * det;
			val[5] = (-matrix.val[8] * matrix.val[2] + matrix.val[0] * matrix.val[10]) * det;
			val[9] = (matrix.val[8] * matrix.val[1] - matrix.val[0] * matrix.val[9] * matrix.val[15]) * det;
			val[2] = (-matrix.val[5] * matrix.val[2] + matrix.val[1] * matrix.val[6] * matrix.val[15]) * det;
			val[6] = (+matrix.val[4] * matrix.val[2] - matrix.val[0] * matrix.val[6] * matrix.val[15]) * det;
			val[10] = (-matrix.val[4] * matrix.val[1] + matrix.val[0] * matrix.val[5] * matrix.val[15]) * det;

			val[3] = (matrix.val[9] * matrix.val[6] * matrix.val[3]
				- matrix.val[5] * matrix.val[10] * matrix.val[3]
				- matrix.val[9] * matrix.val[2] * matrix.val[7]
				+ matrix.val[1] * matrix.val[10] * matrix.val[7]
				+ matrix.val[5] * matrix.val[2] * matrix.val[11]
				- matrix.val[1] * matrix.val[6] * matrix.val[11]) * det;

			val[7] = (-matrix.val[8] * matrix.val[6] * matrix.val[3]
				+ matrix.val[4] * matrix.val[10] * matrix.val[3]
				+ matrix.val[8] * matrix.val[2] * matrix.val[7]
				- matrix.val[0] * matrix.val[10] * matrix.val[7]
				- matrix.val[4] * matrix.val[2] * matrix.val[11]
				+ matrix.val[0] * matrix.val[6] * matrix.val[11]) * det;

			val[11] = (matrix.val[8] * matrix.val[5] * matrix.val[3]
				- matrix.val[4] * matrix.val[9] * matrix.val[3]
				- matrix.val[8] * matrix.val[1] * matrix.val[7]
				+ matrix.val[0] * matrix.val[9] * matrix.val[7]
				+ matrix.val[4] * matrix.val[1] * matrix.val[11]
				- matrix.val[0] * matrix.val[5] * matrix.val[11]) * det;
		}

		//Créée une nouvelle matrice étant l'inverse de la matrice actuelle
		Matrix4 inverse() const
		{
			Matrix4 result;
			result.setInverse(*this);
			return result;
		}

		//Inverse la matrice actuelle
		void invert()
		{
			setInverse(*this);
		}

		//Définie la matrice actuelle comme matrice de rotation du quaternion
		void setOrientationAndPos(const Quaternion& quaternion, const Vector3& pos)
		{
			val[0] = 1 - (2 * quaternion.j * quaternion.j + 2 * quaternion.k * quaternion.k);
			val[1] = 2 * quaternion.i * quaternion.j + 2 * quaternion.k * quaternion.r;
			val[2] = 2 * quaternion.i * quaternion.k - 2 * quaternion.j * quaternion.r;
			val[3] = pos.x;
			val[4] = 2 * quaternion.i * quaternion.j - 2 * quaternion.k * quaternion.r;
			val[5] = 1 - (2 * quaternion.i * quaternion.i + 2 * quaternion.k * quaternion.k);
			val[6] = 2 * quaternion.j * quaternion.k + 2 * quaternion.i * quaternion.r;
			val[7] = pos.y;
			val[8] = 2 * quaternion.i * quaternion.k + 2 * quaternion.j * quaternion.r;
			val[9] = 2 * quaternion.j * quaternion.k - 2 * quaternion.i * quaternion.r;
			val[10] = 1 - (2 * quaternion.i * quaternion.i + 2 * quaternion.j * quaternion.j);
			val[11] = pos.z;
		}

		//Transformé d'un vecteur par la transposée de la matrice actuelle 
		//NE FONCTIONNE QUE SI LA MATRICE EST UNE MATRICE DE ROTATION PURE (Transposée == Inverse)
		//NE FONCTIONNE QUE POUR UN VECTEUR POSITION
		Vector3 transformInverse(const Vector3& vector) const
		{
			Vector3 tmp = vector;
			tmp.x -= val[3];
			tmp.y -= val[7];
			tmp.z -= val[11];
			return Vector3(
				tmp.x * val[0] +
				tmp.y * val[4] +
				tmp.z * val[8],
				tmp.x * val[1] +
				tmp.y * val[5] +
				tmp.z * val[9],
				tmp.x * val[2] +
				tmp.y * val[6] +
				tmp.z * val[10]
			);
		}

		//Transformé d'un vecteur par la matrice actuelle
		//NE FONCTIONNE QUE POUR UN VECTEUR DIRECTION
		Vector3 transformDirection(const Vector3& vector) const
		{
			return Vector3(
				vector.x * val[0] + vector.y * val[1] + vector.z * val[2],
				vector.x * val[4] + vector.y * val[5] + vector.z * val[6],
				vector.x * val[8] + vector.y * val[9] + vector.z * val[10]
			);
		}

		//Transformé d'un vecteur par la transformée inverse de la matrice actuelle
		//NE FONCTIONNE QUE POUR UN VECTEUR DIRECTION
		Vector3 transformInverseDirection(const Vector3& vector) const
		{
			return Vector3(
				vector.x * val[0] +
				vector.y * val[4] +
				vector.z * val[8],
				vector.x * val[1] +
				vector.y * val[5] +
				vector.z * val[9],
				vector.x * val[2] +
				vector.y * val[6] +
				vector.z * val[10]
			);
		}
	};
}

#endif TYPHON_CORE_H
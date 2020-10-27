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
	};
}
#endif TYPHON_CORE_H
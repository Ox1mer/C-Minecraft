#include "Vec3.h"

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator+(const Vec3& other) const {
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const {
	return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(double scalar) const {
	return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(double scalar) const {
	return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3& Vec3::operator+=(const Vec3& other) {
	x += other.x; y += other.y; z += other.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) {
	x -= other.x; y -= other.y; z -= other.z;
	return *this;
}

Vec3& Vec3::operator*=(double scalar) {
	x *= scalar; y *= scalar; z *= scalar;
	return *this;
}

Vec3& Vec3::operator/=(double scalar) {
	x /= scalar; y /= scalar; z /= scalar;
	return *this;
}

bool Vec3::operator==(const Vec3& other) const {
	return x == other.x && y == other.y && z == other.z;
}

bool Vec3::operator!=(const Vec3& other) const {
	return !(*this == other);
}

double Vec3::length() const {
	return std::sqrt(x * x + y * y + z * z);
}

double Vec3::lengthSquared() const {
	return x * x + y * y + z * z;
}

Vec3 Vec3::normalized() const {
	double len = length();
	return len == 0 ? Vec3(0, 0, 0) : *this / len;
}

void Vec3::normalize() {
	double len = length();
	if (len != 0) {
		x /= len; y /= len; z /= len;
	}
}

double Vec3::dot(const Vec3& a, const Vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b) {
	return Vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

Vec3 Vec3::zero() {
	return Vec3(0, 0, 0);
}

Vec3 Vec3::one() {
	return Vec3(1, 1, 1);
}

void Vec3::set(double newX, double newY, double newZ) {
	x = newX; y = newY; z = newZ;
}

void Vec3::print() const {
    std::cout << "3D Vector: X: " << x << ", Y: " << y << ", Z: " << z << std::endl;
}
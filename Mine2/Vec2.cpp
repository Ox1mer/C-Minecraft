#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(double x, double y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
	return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(double scalar) const {
	return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(double scalar) const {
	return Vec2(x / scalar, y / scalar);
}

Vec2& Vec2::operator+=(const Vec2& other) {
	x += other.x; y += other.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
	x -= other.x; y -= other.y;
	return *this;
}

Vec2& Vec2::operator*=(double scalar) {
	x *= scalar; y *= scalar;
	return *this;
}

Vec2& Vec2::operator/=(double scalar) {
	x /= scalar; y /= scalar;
	return *this;
}

bool Vec2::operator==(const Vec2& other) const {
	return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other) const {
	return !(*this == other);
}

double Vec2::length() const {
	return std::sqrt(x * x + y * y);
}

double Vec2::lengthSquared() const {
	return x * x + y * y;
}

Vec2 Vec2::normalized() const {
	double len = length();
	return len == 0 ? Vec2(0, 0) : *this / len;
}

void Vec2::normalize() {
	double len = length();
	if (len != 0) {
		x /= len; y /= len;
	}
}

double Vec2::dot(const Vec2& a, const Vec2& b) {
	return a.x * b.x + a.y * b.y;
}

Vec2 Vec2::zero() {
	return Vec2(0, 0);
}

Vec2 Vec2::one() {
	return Vec2(1, 1);
}

void Vec2::set(double newX, double newY) {
	x = newX; y = newY;
}

void Vec2::print() const {
	std::cout << "2D Vector: X: " << x << ", Y: " << y << std::endl;
}
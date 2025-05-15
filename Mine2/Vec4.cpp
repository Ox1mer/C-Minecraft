#include "Vec4.h"

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}
Vec4::Vec4(double x_, double y_, double z_, double w_) : x(x_), y(y_), z(z_), w(w_) {}

Vec4 Vec4::operator-() const {
    return Vec4(-x, -y, -z, -w);
}

Vec4 Vec4::operator+(const Vec4& other) const {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vec4 Vec4::operator-(const Vec4& other) const {
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vec4 Vec4::operator*(double scalar) const {
    return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4 Vec4::operator/(double scalar) const {
    return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vec4& Vec4::operator+=(const Vec4& other) {
    x += other.x; y += other.y; z += other.z; w += other.w;
    return *this;
}

Vec4& Vec4::operator-=(const Vec4& other) {
    x -= other.x; y -= other.y; z -= other.z; w -= other.w;
    return *this;
}

Vec4& Vec4::operator*=(double scalar) {
    x *= scalar; y *= scalar; z *= scalar; w *= scalar;
    return *this;
}

Vec4& Vec4::operator/=(double scalar) {
    x /= scalar; y /= scalar; z /= scalar; w /= scalar;
    return *this;
}

bool Vec4::operator==(const Vec4& other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Vec4::operator!=(const Vec4& other) const {
    return !(*this == other);
}

double Vec4::length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

double Vec4::lengthSquared() const {
    return x * x + y * y + z * z + w * w;
}

Vec4 Vec4::normalized() const {
    double len = length();
    return len == 0 ? Vec4(0, 0, 0, 0) : *this / len;
}

void Vec4::normalize() {
    double len = length();
    if (len != 0) {
        x /= len; y /= len; z /= len; w /= len;
    }
}

double Vec4::dot(const Vec4& a, const Vec4& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec4 Vec4::zero() {
    return Vec4(0, 0, 0, 0);
}

Vec4 Vec4::one() {
    return Vec4(1, 1, 1, 1);
}

void Vec4::set(double newX, double newY, double newZ, double newW) {
    x = newX; y = newY; z = newZ; w = newW;
}

void Vec4::print() const {
    std::cout << "Vec4: X: " << x << ", Y: " << y << ", Z: " << z << ", W: " << w << std::endl;
}


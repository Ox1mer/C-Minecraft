#pragma once

#include <cmath>
#include <iostream>

#include "Position3D.h"

struct Vec3
{
	double x, y, z;

	Vec3();
	Vec3(double x, double y, double z);

	Vec3 operator-() const;

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator*(double scalar) const;
	Vec3 operator/(double scalar) const;

	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	Vec3& operator*=(double scalar);
	Vec3& operator/=(double scalar);

	bool operator==(const Vec3& other) const;
	bool operator!=(const Vec3& other) const;

	double length() const;
	double lengthSquared() const;
	Vec3 normalized() const;
	void normalize();

	static double dot(const Vec3& a, const Vec3& b);
	static Vec3 cross(const Vec3& a, const Vec3& b);

	static Vec3 zero();   // (0, 0, 0)
	static Vec3 one();    // (1, 1, 1)

	void set(double newX, double newY, double newZ);

	void print() const;
};
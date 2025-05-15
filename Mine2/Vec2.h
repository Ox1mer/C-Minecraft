#pragma once

#include <iostream>
#include <cmath>

struct Vec2
{
	double x, y;

	Vec2();
	Vec2(double x, double y);

	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2 operator*(double scalar) const;
	Vec2 operator/(double scalar) const;

	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);
	Vec2& operator*=(double scalar);
	Vec2& operator/=(double scalar);

	bool operator==(const Vec2& other) const;
	bool operator!=(const Vec2& other) const;

	double length() const;
	double lengthSquared() const;
	Vec2 normalized() const;
	void normalize();

	static double dot(const Vec2& a, const Vec2& b);

	static Vec2 zero(); // (0, 0)
	static Vec2 one();  // (1, 1)

	void set(double newX, double newY);

	void print() const;
};
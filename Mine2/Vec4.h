#pragma once

#include <cmath>
#include <iostream>

struct Vec4
{
    double x, y, z, w;

    Vec4();
    Vec4(double x, double y, double z, double w);

    Vec4 operator-() const;
    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;
    Vec4 operator*(double scalar) const;
    Vec4 operator/(double scalar) const;

    Vec4& operator+=(const Vec4& other);
    Vec4& operator-=(const Vec4& other);
    Vec4& operator*=(double scalar);
    Vec4& operator/=(double scalar);

    bool operator==(const Vec4& other) const;
    bool operator!=(const Vec4& other) const;

    double length() const;
    double lengthSquared() const;
    Vec4 normalized() const;
    void normalize();

    static double dot(const Vec4& a, const Vec4& b);

    static Vec4 zero();
    static Vec4 one();

    void set(double newX, double newY, double newZ, double newW);

    void print() const;
};


#pragma once

#include <array>
#include <iostream>
#include "Vec3.h"
#include "Vec4.h"

struct Mat4 {
    std::array<std::array<double, 4>, 4> data;

    Mat4();
    explicit Mat4(double diagonal);

    static Mat4 identity();
    static Mat4 zero();

    static Mat4 translation(const Vec3& t);
    static Mat4 scale(const Vec3& s);
    static Mat4 rotationX(double angleRad);
    static Mat4 rotationY(double angleRad);
    static Mat4 rotationZ(double angleRad);

    double& operator()(int r, int c);
    double         operator()(int r, int c) const;

    Mat4           operator*(const Mat4& o) const;
    Vec4           operator*(const Vec4& v) const;

    Mat4& operator*=(const Mat4& o);

    Mat4           transpose() const;
    double         determinant() const;
    Mat4           inverse() const;

    void           print() const;
};

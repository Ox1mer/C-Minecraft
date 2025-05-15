#pragma once

#include <iostream>

struct Plane {
    double A, B, C, D;

    Plane();
    Plane(double a, double b, double c, double d);

    void normalize();
    void print() const;
};
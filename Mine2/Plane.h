#pragma once

#include <iostream>
#include <cmath>

struct Plane {
    double A, B, C, D;

    void normalize();
    void print() const;
};


#include "Plane.h"
#include <cmath>

Plane::Plane() : A(0), B(0), C(0), D(0) {}

Plane::Plane(double a, double b, double c, double d)
    : A(a), B(b), C(c), D(d) {
}

void Plane::normalize() {
    double len = std::sqrt(A * A + B * B + C * C);
    if (len != 0.0) {
        A /= len;
        B /= len;
        C /= len;
        D /= len;
    }
}

void Plane::print() const {
    std::cout << "Plane(A=" << A << ", B=" << B
        << ", C=" << C << ", D=" << D << ")\n";
}

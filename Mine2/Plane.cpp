#include "Plane.h"

void Plane::normalize() {
    double len = std::sqrt(A * A + B * B + C * C);
    if (len == 0.0) return;
    A /= len;
    B /= len;
    C /= len;
    D /= len;
}

void Plane::print() const {
    std::cout << "Plane: "
        << "A = " << A << ", "
        << "B = " << B << ", "
        << "C = " << C << ", "
        << "D = " << D << "\n";
}
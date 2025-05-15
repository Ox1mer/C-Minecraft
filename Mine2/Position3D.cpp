#include "Position3D.h"

Position3D::Position3D(double x, double y, double z) : x(x), y(y), z(z) {}

void Position3D::print() const {
	std::cout << "3D Position: X: " << x << "Y: " << y << "Z: " << z << std::endl;
}
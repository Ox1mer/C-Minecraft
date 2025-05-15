#include "Scale3D.h"

Scale3D::Scale3D(double x, double y, double z) : x(x), y(y), z(z) {}

void Scale3D::print() const {
	std::cout << "3D Scale: X: " << x << "Y: " << y << "Z: " << z << std::endl;
}
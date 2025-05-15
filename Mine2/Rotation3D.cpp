#include "Rotation3D.h"

Rotation3D::Rotation3D(double pitch, double yaw, double roll) : pitch(pitch), yaw(yaw), roll(roll) {}

void Rotation3D::print() const {
	std::cout << "3D Rotation: Pitch: " << pitch << "Yaw: " << yaw << "Roll: " << roll << std::endl;
}
#pragma once

#include <iostream>

struct Rotation3D
{
	double pitch, yaw, roll;
	Rotation3D(double pitch = 0, double yaw = 0, double roll = 0);

	void print() const;
};


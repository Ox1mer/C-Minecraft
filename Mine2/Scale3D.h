#pragma once

#include <iostream>


struct Scale3D
{
	double x, y, z;
	Scale3D(double x = 0, double y = 0, double z = 0);

	void print() const;
};


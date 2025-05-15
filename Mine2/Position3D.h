#pragma once

#include <iostream>


struct Position3D
{
	double x, y, z;
	Position3D(double x = 0, double y = 0, double z = 0);

	void print() const;
};


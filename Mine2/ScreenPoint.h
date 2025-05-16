#pragma once


struct ScreenPoint
{
	int x, y;
	double depth;

	ScreenPoint() : x(0), y(0), depth(0.0) {
		
	}

	ScreenPoint(int x, int y, double depth) : x(x), y(y), depth(depth) {

	}
};


#pragma once

#include <array>
#include <vector>
#include "Transform.h"
#include "CubePlane.h"

class Block
{
protected:
	std::array<Position3D, 8> vertices;
	std::array<std::array<int, 3>, 12> triangles;
	std::array<CubePlane, 6> cubePlanes;

private:

public:
	Transform transform;

	Block() : transform(Transform()) {}
	Block(Transform transform);

	void setCubePlaneVisible(int index, bool value);

	std::array<Position3D, 8> getVertices();
	std::array<Position3D, 3> getTriangleGlobalCoords(int a);
	const std::array<CubePlane, 6>& getCubePlanes() const;
	const std::vector<const CubePlane*> getRenderedPlanes() const;
};


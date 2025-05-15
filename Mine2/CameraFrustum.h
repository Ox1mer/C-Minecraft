#pragma once

#include <array>
#include "Mat4.h"
#include "Plane.h"
#include "Position3D.h"

enum FrustumPlaneIndex {
    LEFT = 0,
    RIGHT = 1,
    BOTTOM = 2,
    TOP = 3,
    NEARP = 4,
    FARP = 5
};

class Frustum {
public:
    void update(const Mat4& viewMatrix, const Mat4& projectionMatrix);
    bool isPointInFrustum(const Position3D& point) const;
    const std::array<Plane, 6>& getFrustumPlanes() const;

private:
    std::array<Plane, 6> planes;
};
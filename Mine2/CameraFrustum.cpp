#include "CameraFrustum.h"

void Frustum::update(const Mat4& viewMatrix, const Mat4& projectionMatrix) {
    Mat4 clip = projectionMatrix * viewMatrix;

    planes[LEFT] = Plane(
        clip(3, 0) + clip(0, 0),
        clip(3, 1) + clip(0, 1),
        clip(3, 2) + clip(0, 2),
        clip(3, 3) + clip(0, 3)
    ); planes[LEFT].normalize();

    planes[RIGHT] = Plane(
        clip(3, 0) - clip(0, 0),
        clip(3, 1) - clip(0, 1),
        clip(3, 2) - clip(0, 2),
        clip(3, 3) - clip(0, 3)
    ); planes[RIGHT].normalize();

    planes[BOTTOM] = Plane(
        clip(3, 0) + clip(1, 0),
        clip(3, 1) + clip(1, 1),
        clip(3, 2) + clip(1, 2),
        clip(3, 3) + clip(1, 3)
    ); planes[BOTTOM].normalize();

    planes[TOP] = Plane(
        clip(3, 0) - clip(1, 0),
        clip(3, 1) - clip(1, 1),
        clip(3, 2) - clip(1, 2),
        clip(3, 3) - clip(1, 3)
    ); planes[TOP].normalize();

    planes[NEARP] = Plane(
        clip(3, 0) + clip(2, 0),
        clip(3, 1) + clip(2, 1),
        clip(3, 2) + clip(2, 2),
        clip(3, 3) + clip(2, 3)
    ); planes[NEARP].normalize();

    planes[FARP] = Plane(
        clip(3, 0) - clip(2, 0),
        clip(3, 1) - clip(2, 1),
        clip(3, 2) - clip(2, 2),
        clip(3, 3) - clip(2, 3)
    ); planes[FARP].normalize();
}

bool Frustum::isPointInFrustum(const Position3D& point) const {
    for (const auto& plane : planes) {
        if (plane.A * point.x + plane.B * point.y + plane.C * point.z + plane.D < 0.0) {
            return false;
        }
    }
    return true;
}

const std::array<Plane, 6>& Frustum::getFrustumPlanes() const {
    return planes;
}
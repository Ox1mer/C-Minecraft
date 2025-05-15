#include "Camera.h"

constexpr double PI = 3.14159265358979323846;

static double degreesToRadians(double degrees) {
    return degrees * (PI / 180.0);
}

Camera::Camera(const CameraConfig& cameraConfig) : xPixelSize(cameraConfig.xSize),
yPixelSize(cameraConfig.ySize),
fov(cameraConfig.fov),
nearPlaneDistance(cameraConfig.nearP),
farPlaneDistance(cameraConfig.farP),
transform (
    Position3D(0, 0, 0),
    Rotation3D(0, 0, 0),
    Scale3D(1, 1, 1)
),
worldUpVector(0.0, 1.0, 0.0),
cameraFrustum() {
    aspect = static_cast<double>(xPixelSize) / static_cast<double>(yPixelSize);
    update();
}

void Camera::update() {
    forwardVector = _calculateForwardVector();
    rightVector = _calculateRightVector();
    upVector = _calculateUpVector();

    viewMatrix = _calculateViewMatrix();
    perspectiveMatrix = _calculatePerspectiveMatrix();
    frustumPlanes = _calculateFrustumPlanes();
}

bool Camera::isPointInFrustum(const Position3D& worldPos) {
    return cameraFrustum.isPointInFrustum(worldPos);
}

bool Camera::projectWorldToScreen(const Position3D& pointPos, ScreenPoint& screenPoint) const {
    double vec[4] = { pointPos.x, pointPos.y, pointPos.z, 1.0 };

    Mat4 viewProj = perspectiveMatrix * viewMatrix;

    double clip[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            clip[i] += viewProj.data[i][j] * vec[j];
        }
    }

    double w = clip[3];
    if (std::abs(w) < 1e-6) {
        return false;
    }

    double ndc[3] = { clip[0] / w, clip[1] / w, clip[2] / w };

    if (ndc[0] < -1 || ndc[0] > 1 || ndc[1] < -1 || ndc[1] > 1)
        return false;

    screenPoint.x = (ndc[0] + 1.0) * 0.5 * static_cast<double>(xPixelSize);
    screenPoint.y = (1.0 - ndc[1]) * 0.5 * static_cast<double>(yPixelSize);
    screenPoint.depth = (ndc[2] + 1.0) * 0.5;

    return true;
}

int Camera::getXPixelSizeOfScreen() const {
    return xPixelSize;
}
int Camera::getYPixelSizeOfScreen() const {
    return yPixelSize;
}
const Vec3& Camera::getForwardVector() const {
    return forwardVector;
}
const Vec3& Camera::getRightVector() const {
    return rightVector;
}
const Vec3& Camera::getUpVector() const {
    return upVector;
}

Vec3 Camera::_calculateForwardVector() const {
    double pitchRad = degreesToRadians(transform.getRotation().pitch);
    double yawRad = degreesToRadians(transform.getRotation().yaw);

    double fx = std::cos(pitchRad) * std::sin(yawRad);
    double fy = std::sin(pitchRad);
    double fz = std::cos(pitchRad) * std::cos(yawRad);

    Vec3 forward = Vec3(fx, fy, -fz);
    return forward.normalized();
}

Vec3 Camera::_calculateRightVector() const {
    return Vec3::cross(worldUpVector, forwardVector).normalized();
}

Vec3 Camera::_calculateUpVector() const {
    return Vec3::cross(forwardVector, rightVector).normalized();
}

Mat4 Camera::_calculateViewMatrix() const {
    Vec3 f = forwardVector;
    Vec3 u = upVector;
    Vec3 r = -rightVector;
    Vec3 eye(transform.getPosition().x,
        transform.getPosition().y,
        transform.getPosition().z);

    Mat4 view = Mat4::identity();

    view(0, 0) = r.x;   view(0, 1) = r.y;   view(0, 2) = r.z;   view(0, 3) = -Vec3::dot(r, eye);
    view(1, 0) = u.x;   view(1, 1) = u.y;   view(1, 2) = u.z;   view(1, 3) = -Vec3::dot(u, eye);
    view(2, 0) = -f.x;  view(2, 1) = -f.y;  view(2, 2) = -f.z;  view(2, 3) = Vec3::dot(f, eye);
    view(3, 0) = 0.0;   view(3, 1) = 0.0;   view(3, 2) = 0.0;   view(3, 3) = 1.0;

    return view;
}

Mat4 Camera::_calculatePerspectiveMatrix() const {
    double f = 1.0 / std::tan(degreesToRadians(fov) * 0.5);
    double rangeInv = 1.0 / (nearPlaneDistance - farPlaneDistance);

    Mat4 proj = Mat4::zero();

    proj(0, 0) = f / aspect;
    proj(1, 1) = f;
    proj(2, 2) = (farPlaneDistance + nearPlaneDistance) * rangeInv;
    proj(2, 3) = 2.0 * farPlaneDistance * nearPlaneDistance * rangeInv;
    proj(3, 2) = -1.0;

    return proj;
}


std::array<Plane, 6> Camera::_calculateFrustumPlanes() {
    cameraFrustum.update(viewMatrix, perspectiveMatrix);
    return cameraFrustum.getFrustumPlanes();
}
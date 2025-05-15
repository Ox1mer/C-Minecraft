#pragma once

#include <array>

#include "Transform.h"
#include "Vec2.h"
#include "Vec3.h"
#include "CameraConfig.h"
#include "ScreenPoint.h"
#include "Position3D.h"
#include "Mat4.h"
#include "Plane.h"
#include "CameraFrustum.h"

class Camera
{
private:
	int xPixelSize;
	int yPixelSize;
	double aspect;

protected:
	Vec3 forwardVector;
	Vec3 upVector;
	Vec3 rightVector;
	Frustum cameraFrustum;
	int fov;

	// Distances to frustum planes of camera
	double nearPlaneDistance;
	double farPlaneDistance;

	Vec3 worldUpVector;
	Mat4 viewMatrix;
	Mat4 perspectiveMatrix;
	std::array<Plane, 6> frustumPlanes;

	Vec3 _calculateForwardVector() const;
	Vec3 _calculateRightVector() const;
	Vec3 _calculateUpVector() const;

	Mat4 _calculateViewMatrix() const;
	Mat4 _calculatePerspectiveMatrix() const;
	std::array<Plane, 6> _calculateFrustumPlanes();


public:
	Transform transform;
	
	Camera(const CameraConfig& camConfig);

	void update();
	bool isPointInFrustum(const Position3D& point);
	bool projectWorldToScreen(const Position3D& pointPos, ScreenPoint& screenPoint) const;

	// Getters
	int getXPixelSizeOfScreen() const;
	int getYPixelSizeOfScreen() const;
	const Vec3& getForwardVector() const;
	const Vec3& getRightVector() const;
	const Vec3& getUpVector() const;
};


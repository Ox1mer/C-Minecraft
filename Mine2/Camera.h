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
	CameraFrustum cameraFrustum;
	int fov;

	// Distances to frustum planes of camera
	double nearPlaneDistance;
	double farPlaneDistance;

	Vec3 worldUpVector;
	Mat4 viewMatrix;
	Mat4 perspectiveMatrix;
	std::array<Plane, 6> frustumPlanes;

	Vec3 _calculateForwardVector();
	Vec3 _calculateRightVector();
	Vec3 _calculateUpVector();

	Mat4 _getViewMatrix();
	Mat4 _getPerspectiveMatrix();
	std::array<Plane, 6> _getFrustumPlanes();


public:
	Transform transform;
	
	Camera(const CameraConfig& camConfig);

	void update();
	bool isPointInFrustum(const Position3D& point);
	bool projectWorldToScreen(const Position3D& pointPos, ScreenPoint& screenPoint) const;

	// Getters
	int getXPixelSizeOfScreen() const;
	int getYPixelSizeOfScreen() const;
	Vec3 getForwardVector() const;
	Vec3 getRightVector() const;
	Vec3 getUpVector() const;
};


#pragma once

#include "Position3D.h"
#include "Rotation3D.h"
#include "Scale3D.h"

struct Transform
{
public:
	Position3D location;
	Rotation3D rotation;
	Scale3D scale;

	Transform();
	Transform(const Position3D& location, const  Rotation3D& rotation, const  Scale3D& scale);

	// Setters
	void setPosition(double x, double y, double z);
	void setRotation(double pitch, double yaw, double roll);
	void setScale(double x, double y, double z);

	// Getters
	Position3D getPosition() const;
	Rotation3D getRotation() const;
	Scale3D getScale() const;
};


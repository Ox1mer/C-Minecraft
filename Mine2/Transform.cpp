#include "Transform.h"

Transform::Transform() :
    location(0.0, 0.0, 0.0),
    rotation(0.0, 0.0),
    scale(1.0, 1.0, 1.0) {
}

Transform::Transform(const Position3D& pos, const Rotation3D& rot, const Scale3D& sc)
    : location(pos), rotation(rot), scale(sc) {
}

// Setters
void Transform::setPosition(double x, double y, double z) {
    location = Position3D(x, y, z);
}
void Transform::setRotation(double pitch, double yaw, double roll) {
    rotation = Rotation3D(pitch, yaw, roll);
}
void Transform::setScale(double x, double y, double z) {
    scale = Scale3D(x, y, z);
}

// Getters
Position3D Transform::getPosition() const { return location; }
Rotation3D Transform::getRotation() const { return rotation; }
Scale3D Transform::getScale() const { return scale; }
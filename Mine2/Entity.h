#pragma once

#include "Camera.h"
#include "Transform.h"

class Entity
{
protected:
	int health;
	int speed;
	
public:
	Transform transform;

	Entity(int health, int speed, Transform transform);

	void damage(int health);
	void kill();

	// Setters
	void setSpeed(int newSpeed);
	void setHealth(int newHealth);

	// Getters
	int getHealth() const;
	int getSpeed() const;
};


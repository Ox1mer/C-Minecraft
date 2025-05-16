#include "Entity.h"

Entity::Entity(int health, int speed, Transform transform) : 
	health(health), speed(speed), transform(transform) {

}

void Entity::kill() {

}

void Entity::damage(int health) {

}

void Entity::setSpeed(int newSpeed) {
	speed = newSpeed;
}

void Entity::setHealth(int newHealth) {
	health = newHealth;
}

int Entity::getHealth() const {
	return health;
}

int Entity::getSpeed() const {
	return speed;
}
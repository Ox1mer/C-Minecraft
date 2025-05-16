#include "Player.h"

Player::Player(int health, int speed, Camera& camera, Transform transform)
    : Entity(health, speed, transform), camera(camera)
{
    
}

void Player::movePlayer(double x, double y, double z, int where) {
    transform.setPosition(
        transform.getPosition().x + x * where,
        transform.getPosition().y + y * where,
        transform.getPosition().z + z * where
    );
    camera.transform.setPosition(
        transform.getPosition().x,
        transform.getPosition().y + eyeHigh,
        transform.getPosition().z
    );
}

const Camera& Player::getCamera() const {
    return camera;
}
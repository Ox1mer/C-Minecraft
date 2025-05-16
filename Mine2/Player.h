#pragma once

#include "Entity.h"

class Player : public Entity
{
private:
    double eyeHigh = 1.8;
    Camera& camera;

public:
    Player(int health, int speed, Camera& camera, Transform transform);

    void movePlayer(double x, double y, double z, int where);

    // Getters
    const Camera& getCamera() const;
};


#pragma once

struct CameraConfig {
    int ySize;
    int xSize;
    int fov;
    double nearP;
    double farP;

    CameraConfig(int ySize = 1080, int xSize = 1920, int fov = 90, double nearP = 0.001, double farP = 1000.0);
};
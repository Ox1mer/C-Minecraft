#include "Block.h"

Block::Block(Transform transform) : transform(transform) {
    vertices = {
        Position3D{transform.getPosition().x,     transform.getPosition().y,     transform.getPosition().z},     // 1
        Position3D{transform.getPosition().x + 1, transform.getPosition().y,     transform.getPosition().z},     // 2
        Position3D{transform.getPosition().x + 1, transform.getPosition().y,     transform.getPosition().z + 1}, // 3
        Position3D{transform.getPosition().x,     transform.getPosition().y,     transform.getPosition().z + 1}, // 4
        Position3D{transform.getPosition().x,     transform.getPosition().y + 1, transform.getPosition().z},     // 5
        Position3D{transform.getPosition().x + 1, transform.getPosition().y + 1, transform.getPosition().z},     // 6
        Position3D{transform.getPosition().x + 1, transform.getPosition().y + 1, transform.getPosition().z + 1}, // 7
        Position3D{transform.getPosition().x,     transform.getPosition().y + 1, transform.getPosition().z + 1}  // 8
    };

    // Order is very important for backface culling
    triangles = {
        std::array<int, 3>{1, 2, 6},
        std::array<int, 3>{1, 6, 5},
        std::array<int, 3>{7, 3, 4},
        std::array<int, 3>{8, 7, 4},
        std::array<int, 3>{1, 5, 8},
        std::array<int, 3>{1, 8, 4},
        std::array<int, 3>{2, 3, 7},
        std::array<int, 3>{2, 7, 6},
        std::array<int, 3>{1, 2, 3},
        std::array<int, 3>{1, 3, 4},
        std::array<int, 3>{5, 6, 7},
        std::array<int, 3>{5, 7, 8}
    };


    cubePlanes = {
        CubePlane{0, 1, true},
        CubePlane{2, 3, true},
        CubePlane{4, 5, true},
        CubePlane{6, 7, true},
        CubePlane{8, 9, true},
        CubePlane{10, 11, true}
    };
}


void Block::setCubePlaneVisible(int index, bool value) {
    cubePlanes[index].render = value;
}

std::array<Position3D, 8> Block::getVertices() {
    return vertices;
}

std::array<Position3D, 3> Block::getTriangleGlobalCoords(int a) {
    std::array<Position3D, 3> newTriangle;

    for (int i = 0; i < 3; ++i) {
        int vertIndex = triangles[a][i] - 1;
        newTriangle[i] = vertices[vertIndex];
    }

    return newTriangle;
}

const std::array<CubePlane, 6>& Block::getCubePlanes() const {
    return cubePlanes;
}

const std::vector<const CubePlane*> Block::getRenderedPlanes() const {
    std::vector<const CubePlane*> renderedCubePlanes;
    for (const CubePlane& cubePlane : cubePlanes) {
        if (cubePlane.render) {
            renderedCubePlanes.push_back(&cubePlane);
        }
    }
    return renderedCubePlanes;
}
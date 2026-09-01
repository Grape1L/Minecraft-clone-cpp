#pragma once

#include <OpenGL/Math/Vec3.h>
#include <iostream>

class Debug {
public:
    static void printVec3(const Vec3 &e) {
        std::cout << "x: " << e.x << " y: " << e.y << " z: " << e.z << "\n";
    }
};
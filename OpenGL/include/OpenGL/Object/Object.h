#pragma once

#include <OpenGL/Math/Mat4.h>
#include <OpenGL/Math/Vec3.h>

class Object {
public:
    Object();
    ~Object();

public:
    Mat4 modelMatrix(const Vec3 &position, const Vec3 &rotation, const Vec3 &scale);
};
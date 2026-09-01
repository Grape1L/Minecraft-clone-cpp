#pragma once

#include <cmath>

class Vec3 {
public:
    Vec3() {};
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
public:
    Vec3 operator - (const Vec3 &other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator + (const Vec3 &other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator * (const Vec3 &other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    bool operator == (const Vec3 &other) const {
        return (x == other.x && y == other.y && z == other.z);
    }


    
    Vec3 operator * (int value) const {
        return Vec3(x * value, y * value, z * value);
    }

    Vec3 operator * (float value) const {
        return Vec3(x * value, y * value, z * value);
    }

    Vec3 operator + (int value) const {
        return Vec3(x + value, y + value, z + value);
    }

    Vec3 operator + (float value) const {
        return Vec3(x + value, y + value, z + value);
    }

    Vec3 operator - (float value) const {
        return Vec3(x - value, y - value, z - value);
    }

    Vec3 operator / (int value) const {
        return Vec3(x / value, y / value, z / value);
    }

    Vec3 operator % (int value) const {
        return Vec3((int)x % value, (int)y % value, (int)z % value);
    }

public:
    static float length(const Vec3 &v) {
        return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    static Vec3 normalize(const Vec3 &v) {
        float len = length(v);
        if(len == 0) {
            return Vec3(0,0,0); // zrobic zeby returnowalo cos innego
        }
        return Vec3(v.x/len, v.y/len, v.z/len);
    }

    static float dot(const Vec3 &v1, const Vec3 &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    static Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
        return Vec3(
            v1.y*v2.z - v1.z*v2.y, 
            v1.z*v2.x - v1.x*v2.z, 
            v1.x*v2.y - v1.y*v2.x
        );
    }

    static Vec3 floor(const Vec3 &v) {
        return Vec3(std::floor(v.x), std::floor(v.y), std::floor(v.z));
    }

public:
    float x=0, y=0, z=0;
};
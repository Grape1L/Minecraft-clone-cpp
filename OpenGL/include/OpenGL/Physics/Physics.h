#pragma once

struct AABB {
    Vec3 min;
    Vec3 max;
};

class Physics {
public:
    static void implementGravity(float &yPos, float gAcceleration, float &vy, float dt) {
        vy += gAcceleration * dt;
        yPos += vy * dt;
    }

    static bool checkCollisions(const Vec3 &a, const Vec3& aSize, const Vec3 &b, const Vec3 &bSize = Vec3(0.5f, 0.5f, 0.5f)) {
        return (
            a.x - aSize.x <= b.x + bSize.x &&
            a.x + aSize.x >= b.x - bSize.x &&
            a.y - aSize.y <= b.y + bSize.y &&
            a.y + aSize.y >= b.y - bSize.y &&
            a.z - aSize.z <= b.z + bSize.z && 
            a.z + aSize.z >= b.z - bSize.z
        );
    }

	static bool AABBcheckCollisions(const AABB& a, const AABB& b) {
        return (
            a.min.x <= b.max.x &&
            a.max.x >= b.min.x &&
            a.min.y <= b.max.y &&
            a.max.y >= b.min.y &&
            a.min.z <= b.max.z &&
            a.max.z >= b.min.z
        );
	}
};
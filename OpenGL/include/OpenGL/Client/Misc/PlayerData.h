#pragma once

#pragma pack(push, 1)

#include <OpenGL/Math/Vec3.h>
#include <vector>
#include <OpenGL/World/Chunk.h>
#include <cstdint>

struct PlayerData {
    Vec3 playerPos;
    float yaw;
    float pitch;
};

#pragma pack(pop)
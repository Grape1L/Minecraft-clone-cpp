#pragma once

#include <vector>
#include <OpenGL/Math/Vec3.h>
#include <OpenGL/Math/Vec2.h>
#include <OpenGL/Math/Vec2i.h>

#include <OpenGL/Blocks/Block.h>
#include <cstdint>

class Chunk {
public:
    Chunk();
    ~Chunk();

public:
    Vec2i chunkPos;

public:
    static constexpr inline uint8_t chunkSize = 16;
    static constexpr inline uint8_t chunkYSize = 128;

public:
    Block blocks[chunkSize][chunkYSize][chunkSize];
};
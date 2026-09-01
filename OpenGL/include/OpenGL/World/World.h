#pragma once

#include <vector>
#include <memory>
#include <OpenGL/Types.h>
#include <OpenGL/Math/Vec2i.h>

class Chunk;

class World {
public:
    World();
    ~World();

protected:
    std::unique_ptr<Chunk> chunkPTR;

public:
    allChunksType allChunks;
	allChunksType::iterator chunkAt(const Vec3& pos);

public:
    static constexpr float gravityAcceleration = -0.0017f;

public:
	int worldSize = 32; // in chunks

public:
    uint8_t getChunkSize();
    uint8_t getChunkHeight();
};
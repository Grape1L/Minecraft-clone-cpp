#pragma once

#include <OpenGL/Math/Vec3.h>
#include <vector>
#include <memory>
#include <OpenGL/Types.h>

class World;
class Chunk;

class WorldGen {
public:
    WorldGen();
    ~WorldGen();

public:
    void generateFlatWorld(World& world);

protected:
    std::unique_ptr<Chunk> chunkPTR;
};
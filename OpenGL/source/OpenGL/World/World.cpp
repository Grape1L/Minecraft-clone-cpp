#include <OpenGL/World/World.h>
#include <OpenGL/World/Chunk.h>

World::World() {
    chunkPTR = std::make_unique<Chunk>();
}

World::~World() {
    
}

allChunksType::iterator World::chunkAt(const Vec3& pos)
{
    Vec2i key = Vec2i(
        std::floor(pos.x / 16) * 16,
        std::floor(pos.z / 16) * 16
    );
    auto it = this->allChunks.find(key);

    return it;
}

uint8_t World::getChunkSize()
{
    return chunkPTR->chunkSize;
}

uint8_t World::getChunkHeight()
{
    return chunkPTR->chunkYSize;
}
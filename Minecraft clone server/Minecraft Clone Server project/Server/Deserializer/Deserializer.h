#pragma once

#include <vector>
#include <OpenGL/World/Chunk.h>
#include <cstring>

inline void deserializeData(const char*& data, size_t size, void* dest) {
    memcpy(dest, data, size);
    data += size;
}

inline void deserializeChunk(const char*& data, Chunk& chunk) {
    deserializeData(data, sizeof(chunk.chunkPos), &chunk.chunkPos);

    size_t objectsCount;
    deserializeData(data, sizeof(objectsCount), &objectsCount);

    //chunk.blocks.resize(objectsCount);
    //if (objectsCount > 0) {
    //    deserializeData(data, objectsCount * sizeof(Vec3), chunk.blocks.data());
    //}
}
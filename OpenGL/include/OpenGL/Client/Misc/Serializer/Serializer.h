#pragma once

#include <vector>
#include <OpenGL/World/Chunk.h>

inline void appendSerializeData(const void* data, size_t size, std::vector<char>& buffer) {
    const char* ptr = (const char*)data;
    buffer.insert(buffer.end(), ptr, ptr + size);
}

inline void appendSerializeChunk(const Chunk& chunk, std::vector<char>& buffer) {
    appendSerializeData(&chunk.chunkPos, sizeof(chunk.chunkPos), buffer);

    //size_t objectsCount = chunk.blocks.size();
    //appendSerializeData(&objectsCount, sizeof(objectsCount), buffer);
    //
    //appendSerializeData(chunk.blocks.data(), objectsCount * sizeof(Vec3), buffer);
}
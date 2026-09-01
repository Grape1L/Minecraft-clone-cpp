#pragma once

#include <vector>
#include <OpenGL/World/Chunk.h>

struct DataHeader {
    std::vector<Chunk> allChunks;
};
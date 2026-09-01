#include <OpenGL/World/WorldGen.h>
#include <OpenGL/World/Chunk.h>
#include <OpenGL/World/World.h>

#include <iostream>

WorldGen::WorldGen() {
    chunkPTR = std::make_unique<Chunk>();
}

WorldGen::~WorldGen() {

}

void WorldGen::generateFlatWorld(World& world) {
    const size_t chunkSize = chunkPTR->chunkSize;

    size_t start_x = 0; size_t start_z = 0;

    const size_t chunks = world.worldSize;

    for (int numChunksRow = 0; numChunksRow < chunks / 2; numChunksRow++) {
        for (int numChunksCol = 0; numChunksCol < chunks / 2; numChunksCol++) {

            auto newChunk = std::make_unique<Chunk>();
            newChunk->chunkPos = Vec2i(start_x, start_z);

            for (size_t y = 0; y < chunkPTR->chunkYSize; y++) {
                for (size_t x = 0; x < chunkSize; x++) {
                    for (int z = 0; z < chunkSize; z++) {
                        Block newBlock{
                            BlockType::Air
                        };

                        if (y == 0) {
                            newBlock.blockType = BlockType::Grass;
                        }

                        newChunk->blocks[x][y][z] = newBlock;
                    }
                }
            }

            world.allChunks[newChunk->chunkPos] = std::move(newChunk);
            start_z += chunkSize;
        }

        start_x += chunkSize;
        start_z = 0;
    }
}
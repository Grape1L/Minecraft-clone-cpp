#pragma once

#include <OpenGL/Math/Vec2i.h>
#include <OpenGL/Blocks/Block.h>
#include <OpenGL/World/Chunk.h>

struct InitChunk
{
	Vec2i chunkPos;
	Block blocks[Chunk::chunkSize][3][Chunk::chunkSize];
};
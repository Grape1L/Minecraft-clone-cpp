#pragma once

#include <OpenGL/World/World.h>
#include <OpenGL/World/Chunk.h>


class ChunkMeshing
{
public:
	ChunkMeshing();
	~ChunkMeshing();

public:
	void MeshChunk(Chunk& chunk);
};
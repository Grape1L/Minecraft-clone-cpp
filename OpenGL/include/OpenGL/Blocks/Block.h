#pragma once

#include <OpenGL/Math/Vec3.h>

enum class BlockType : unsigned char {
	Air,
	Grass,
	Stone,
};

struct Block {
	BlockType blockType;
};
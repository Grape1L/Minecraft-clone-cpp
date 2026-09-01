#pragma once
#pragma pack(push, 1)

#include <OpenGL/Math/Vec3.h>
#include <cstdint>
#include <OpenGL/Math/Vec2i.h>
#include <OpenGL/Blocks/Block.h>
#include <variant>

enum class ActionType {
    PlaceBlock, 
    BreakBlock,
    DamagePlayer,
};

struct BlockAction {
    ActionType actionType;
    Vec3 position;
    Vec2i chunkPos;
    uint32_t objectNum;
    BlockType blockType;
};

struct DamageAction {
    int damagedPlayerID;
    float damage;
};

struct Action {
    ActionType actionType;

    std::variant<BlockAction, DamageAction> data;
};

#pragma pack(pop)
#pragma once

#include <unordered_map>
#include <OpenGL/World/Chunk.h>
#include <OpenGL/Math/Vec2i.h>
#include <memory>
#include <OpenGL/Client/Misc/PlayerData.h>


using allChunksType = std::unordered_map<Vec2i, std::unique_ptr<Chunk>>;
using nearbyChunksType = std::vector<Chunk*>;
using AllPlayersDataType = std::unordered_map<size_t, PlayerData>;
#pragma once

#include <OpenGL/Math/Vec3.h>
#include <vector>
#include <optional>
#include <string>
#include <OpenGL/World/Chunk.h>
#include <OpenGL/World/World.h>
#include <OpenGL/Debug.h>
#include <OpenGL/Types.h>
#include <variant>
#include <algorithm>
#include <OpenGL/Physics/Physics.h>


enum class Face : uint8_t {
    posX = 0, negX, posY, negY, posZ, negZ
};

//enum class TypeHit {
//    player, block
//};

struct HitBlock {
    Chunk* chunk;
    Vec3 position;
    Face face;
};

struct HitEntity {
    size_t playerID;
};

using Hit = std::variant<HitBlock, HitEntity>;

class RayCast {
public:
    static std::optional<Hit> rayCast(World& world, const Vec3 &origin, const Vec3 &normalizedDir, const AllPlayersDataType& players,  float maxDistance, float step = 0.05f) {

        for (float travelled = 0.0f; travelled <= maxDistance; travelled += step) {
            Vec3 point = origin - normalizedDir * travelled;

            Vec3 roundedPoint = Vec3(
                (int)std::round(point.x), (int)std::round(point.y), (int)std::round(point.z)
            );
            Vec3 roundedPointLocal = roundedPoint % 16;

            Chunk* chunk = world.chunkAt(roundedPoint)->second.get();

            Block blockHit = chunk->blocks[ (int)roundedPointLocal.x ] [ (int)roundedPointLocal.y ] [ (int)roundedPointLocal.z ];

            if (blockHit.blockType != BlockType::Air) {

                Vec3 t = point - roundedPoint;

                float biggest = std::max(t.x, t.y);
                biggest = std::max(biggest, t.z);

                float smallest = std::min(t.x, t.y);
                smallest = std::min(smallest, t.z);

                Face face;

                if (fabs(smallest) >= biggest) {
                    if (smallest == t.x) face = Face::negX;
                    if (smallest == t.y) face = Face::negY;
                    if (smallest == t.z) face = Face::negZ;
                }
                else {
                    if (biggest == t.x) face = Face::posX;
                    if (biggest == t.y) face = Face::posY;
                    if (biggest == t.z) face = Face::posZ;
                }

                return std::optional<Hit>{ HitBlock{ chunk, roundedPointLocal, face } };
            }


            for (auto& [id, player] : players) {
                Vec3 playerMin{ player.playerPos.x - 0.2f, player.playerPos.y - 1.5f, player.playerPos.z - 0.2f };
                Vec3 playerMax{ player.playerPos.x + 0.2f, player.playerPos.y, player.playerPos.z + 0.2f };

                if (Physics::AABBcheckCollisions(AABB(point, point), AABB(playerMin, playerMax))) {
                    return std::optional<Hit>{ HitEntity{ id } };
                }
            }
        }
        return std::nullopt;
    }
};
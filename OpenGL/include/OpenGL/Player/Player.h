#pragma once

#include <OpenGL/Window/Window.h>
#include <OpenGL/Math/Mat4.h>
#include <OpenGL/Math/Vec3.h>
#include <vector>
#include <chrono>
#include <OpenGL/Types.h>
#include <OpenGL/Player/Inventory.h>
#include <OpenGL/Physics/RayCast.h>

class Camera;
class World;
class Chunk;
class Client;
class Inventory;


class Player {
public:
    Player(Client& client, World& _world, AllPlayersDataType& _allPlayersData);
    ~Player();

    std::unique_ptr<Camera> camera;
	std::unique_ptr<Inventory> m_inventory;
    

    bool inAir = true;

    float vy = 0;

protected:
    World& world;

public:
    void onUpdate(Mat4 &view, std::chrono::duration<float> deltaTime, bool focused);

    void checkCollisions();

    Rect displaySize;

    void handleLeftClick(const Vec3& dir);

    void placeBlock(const Vec3 &cameraPos, const Vec3 &dir);
    void breakBlock(HitBlock& hit);

    void damageEntity(const HitEntity& hitEntity, float damage);

public:
    const float playerHeight = 1.5f;
    const float playerFatness = 0.2f;
    const float playerRange = 4.5f;

private:

    bool helperInAir = true;


private:

    int Bplace = 60;
    int Bbreak = 60;


private:
    Client& m_client;
	AllPlayersDataType& allPlayersData;

public:
    nearbyChunksType nearbyCollisionChunks;
    nearbyChunksType getNeabyChunks(const Vec3& pos, int axisSize);

public:
    bool isPlayerOccupyingBlock(const Vec3& playerPos, const Vec3& newObject);

private:
	AABB getHitBox(const Vec3& playerPos, const AABB& size = AABB(Vec3(0.5, 0.5, 0.5), Vec3(0.5, 0.5, 0.5)));
    AABB hitBox;

public:
	ItemStack* itemInHand;

public:
    int playerHealth = 100; // clamp it later and create separate classes for health etc
};
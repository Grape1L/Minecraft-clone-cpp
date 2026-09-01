#include <OpenGL/Player/Player.h>
#include <iostream>
#include <OpenGL/Camera/Camera.h>
#include <OpenGL/World/World.h>
#include <OpenGL/Client/Client.h>
#include <windows.h>
#include <OpenGL/Physics/Physics.h>
#include <OpenGL/Debug.h>
#include <OpenGL/Client/Misc/ActionHeader.h>
#include <OpenGL/Client/Misc/PlayerData.h>
#include <OpenGL/Client/Misc/Packet.h>
#include <variant>


Player::Player(Client& _client, World& _world, AllPlayersDataType& _allPlayersData) : m_client(_client), world(_world), allPlayersData(_allPlayersData) {
    m_inventory = std::make_unique<Inventory>();
    camera = std::make_unique<Camera>(*this);

    itemInHand = &m_inventory->getItemAtSlot(m_inventory->currentSlot);
    

    camera->y = 3;

    camera->x = world.worldSize / 2;
    camera->z = world.worldSize / 2;

    
    m_inventory->addItemToInventory(ItemStack{ BlockType::Grass, 1 }, 0);
    m_inventory->addItemToInventory(ItemStack{ BlockType::Stone, 255 }, 1);

    ItemStack pistol;
    pistol.item = Weapon::pistol;
    pistol.count = 1;
    m_inventory->addItemToInventory(pistol, 3);

    ItemStack woodenSword;
    woodenSword.item = Weapon::woodenSword;
    woodenSword.count = 1;
    m_inventory->addItemToInventory(woodenSword, 4);

}

Player::~Player() {
    
}

void Player::onUpdate(Mat4 &view, std::chrono::duration<float> deltaTime, bool focused) 
{
    Physics::implementGravity(camera->y, world.gravityAcceleration, this->vy, deltaTime.count());


    this->checkCollisions();

    if (inAir == false) {
        this->vy=0;
    }


    Vec3 front = camera->getFront();
    view = camera->lookAt(camera->getDataXYZ(), camera->getDataXYZ() - front, Vec3(0,2,0));


// Send player data

    PlayerData playerData = PlayerData{
        camera->getDataXYZ(), camera->yaw, camera->pitch
    };

    
    if (m_client.piggyAckPackets.size() != 0) {
        m_client.sendPacket(
            Packet{
                0, PacketType::PACKET_PLAYER_STATE, playerData, m_client.piggyAckPackets[0]
            }
        );
        m_client.piggyAckPackets.erase(m_client.piggyAckPackets.begin());
    }
    else {
        m_client.sendPacket(
            Packet{
                0, PacketType::PACKET_PLAYER_STATE, playerData
            }
        );
    }

    if (this->playerHealth <= 0) {
        camera->y = 120; // for now
    }

// Send player data

    if (!focused) return;

    camera->listenForInput(deltaTime);
    camera->mouseInput(0.2f, displaySize.width, displaySize.height);

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        if (Bplace / 60 == 1) {
            placeBlock(camera->getDataXYZ(), front);
        }
        Bplace -= 1;

        if (Bplace <= 0) {
            Bplace = 60;
        }
    }
    else {
        Bplace = 60;
    }

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        if (Bbreak / 60 == 1) {
            handleLeftClick(front);
        }
        Bbreak -= 1;

        if (Bbreak <= 0) {
            Bbreak = 60;
        }
    }
    else {
        Bbreak = 60;
    }
}

void Player::checkCollisions() {
    for (int x = camera->x - 1; x < camera->x + 1; x++) {
        for (int y = camera->y - 2; y < camera->y + 1; y++) {
            for (int z = camera->z - 1; z < camera->z + 1; z++) {

                Vec3 roundedBlockPos = Vec3(
                    std::round(x), std::round(y), std::round(z)
                );
                Vec3 roundedBlockPosLocal = roundedBlockPos % 16;


                Chunk* chunk = world.chunkAt(roundedBlockPos)->second.get();

                Block block = chunk->blocks[(int)roundedBlockPosLocal.x][(int)roundedBlockPosLocal.y][(int)roundedBlockPosLocal.z];
                if (block.blockType == BlockType::Air) continue;

                if (Physics::checkCollisions(
                    Vec3(camera->x, camera->y - playerHeight, camera->z),
                    Vec3(0, 0, 0),
                    roundedBlockPos,
                    Vec3(0.5f, 0.5f, 0.5f)
                ))
                {

                    camera->y = roundedBlockPos.y + 0.5f + playerHeight;
                    helperInAir = false;
                }

                else if (Physics::AABBcheckCollisions(
                    getHitBox(
                        camera->getDataXYZ(),
                        AABB(
                            Vec3(0, 0.2f, 0),
                            Vec3(0, 0.2f, 0)
                        )
                    ),
                    getHitBox(roundedBlockPos)
                )
                    )
                {
                    camera->y = roundedBlockPos.y - (0.5f + 0.2f);
                    this->vy = 0;
                }

                if (Physics::AABBcheckCollisions(
                    getHitBox(
                        camera->getDataXYZ(),
                        AABB(
                            Vec3(playerFatness, playerHeight - 0.01, playerFatness),
                            Vec3(playerFatness, 0, playerFatness)
                        )
                    ),
                    getHitBox(roundedBlockPos)
                    )
                    )
                {
                    Vec3 distance = camera->getDataXYZ() - roundedBlockPos;

                    float biggest = fabs(distance.x) > fabs(distance.z) ? distance.x : distance.z;

                    float offset = 0.5f + playerFatness;
                    if (biggest < 0) offset = -offset;

                    if (biggest == distance.x) {
                        camera->x = roundedBlockPos.x + offset;
                    }
                    else if (biggest == distance.z) {
                        camera->z = roundedBlockPos.z + offset;
                    }
                }
            }
        }
    }

    if (helperInAir == false) {
        inAir = false;
    }
    else {
        inAir = true;
    }
    helperInAir = true;
}

void Player::handleLeftClick(const Vec3& dir)
{
    Weapon::Weapon* weaponInHand = itemInHand->getWeapon();

    float range = playerRange;
    if (weaponInHand) {
        range = weaponInHand->range;
    }

    std::optional<Hit> hit = RayCast::rayCast(world, camera->getDataXYZ(), dir, allPlayersData, range, 0.005f);
    if (hit == std::nullopt) {
        return;
    }

    if (HitEntity* hitEntity = std::get_if<HitEntity>(&*hit)) {
        if (weaponInHand) {
            this->damageEntity(*hitEntity, weaponInHand->damage);
        }
    }
    else if (HitBlock* hitBlock = std::get_if<HitBlock>(&*hit)) {
        if (weaponInHand == nullptr) {
            this->breakBlock(*hitBlock);
        }
    }
}

void Player::placeBlock(const Vec3 &cameraPos, const Vec3 &dir) {
    std::optional<Hit> result = RayCast::rayCast(world, cameraPos, dir, allPlayersData, playerRange, 0.005f);
	
    if (result == std::nullopt) return;

    HitBlock* hit = std::get_if<HitBlock>(&*result);
    if (!hit) return;

    Debug::printVec3(hit->position);

	Block hitBlock = hit->chunk->blocks[(int)hit->position.x][(int)hit->position.y][(int)hit->position.z];

    Block newObject = hitBlock;
    Vec3 newObjectPos = hit->position;

    if (!itemInHand) return;

    BlockType* blockTypePTR = itemInHand->getBlock();
    if (!blockTypePTR) return;

    BlockType blockType = *blockTypePTR;
    if (blockType == BlockType::Air) return; 
    

	newObject.blockType = blockType;

    std::unique_ptr<Chunk>::pointer correctChunk = world.chunkAt(
        Vec3(newObjectPos.x + hit->chunk->chunkPos.x,
            newObjectPos.y,
            newObjectPos.z + hit->chunk->chunkPos.y
        )
    )->second.get();

    size_t chunkSize = world.getChunkSize();

    if (hit->face == Face::posX) {
        newObjectPos.x += 1;
        if (newObjectPos.x == 16) {
            correctChunk = world.chunkAt(
                Vec3(newObjectPos.x + hit->chunk->chunkPos.x,
                    newObjectPos.y,
                    newObjectPos.z + hit->chunk->chunkPos.y
                )
            )->second.get();
            newObjectPos.x = 0;
        }
    }
    else if (hit->face == Face::posY) newObjectPos.y += 1;
    else if (hit->face == Face::posZ) {
        newObjectPos.z += 1;
        if (newObjectPos.z == 16) {
            correctChunk = world.chunkAt(
                Vec3(newObjectPos.x + hit->chunk->chunkPos.x,
                    newObjectPos.y,
                    newObjectPos.z + hit->chunk->chunkPos.y
                )
            )->second.get();
            newObjectPos.z = 0;
        }
    }

    else if (hit->face == Face::negX) {
        newObjectPos.x -= 1;
        if (newObjectPos.x == -1) {
            correctChunk = world.chunkAt(
                Vec3(newObjectPos.x + hit->chunk->chunkPos.x,
                    newObjectPos.y,
                    newObjectPos.z + hit->chunk->chunkPos.y
                )
            )->second.get();
            newObjectPos.x = chunkSize - 1;
        }
    }
    else if (hit->face == Face::negY) newObjectPos.y -= 1;
    else if (hit->face == Face::negZ) {
        newObjectPos.z -= 1;
        if (newObjectPos.z == -1) {
            correctChunk = world.chunkAt(
                Vec3(newObjectPos.x + hit->chunk->chunkPos.x,
                    newObjectPos.y,
                    hit->chunk->chunkPos.y - chunkSize
                )
            )->second.get();
            newObjectPos.z = chunkSize - 1;
        }
    }

    if (newObjectPos == hit->position) {
        return;
    }


    Vec3 newObjectRealPos = newObjectPos + Vec3(hit->chunk->chunkPos.x, 0, hit->chunk->chunkPos.y);
	bool collission = this->isPlayerOccupyingBlock(camera->getDataXYZ(), newObjectRealPos);
    if (collission) return;

    for (const auto& [id, playerState] : allPlayersData) {
        if (this->isPlayerOccupyingBlock(playerState.playerPos, newObjectRealPos))
            return;
    }

    correctChunk->blocks[(size_t)newObjectPos.x][(size_t)newObjectPos.y][(size_t)newObjectPos.z] = newObject;

    m_inventory->removeCountFromItemAtSlot(m_inventory->currentSlot, 1);

    // SERVER

    BlockAction blockAction = BlockAction{
        ActionType::PlaceBlock, newObjectPos, correctChunk->chunkPos, 0, newObject.blockType
    };

    m_client.sendPacket(Packet{
        0, PacketType::PACKET_ACTION, Action{ ActionType::PlaceBlock, blockAction }
    });

    // SERVER
}

void Player::breakBlock(HitBlock& hit) {

    Block& hitBlock = hit.chunk->blocks[(int)hit.position.x][(int)hit.position.y][(int)hit.position.z];

    // Add item to inventory
    bool itemFound = false;
    for (int i = 0; i < m_inventory->TOTAL_SLOT_COUNT; i++) {
        BlockType* blockType = m_inventory->getItemAtSlot(i).getBlock();
        if (!blockType) continue;

        if (*blockType == hitBlock.blockType) {
            m_inventory->addItemToInventory(ItemStack(hitBlock.blockType, m_inventory->getItemAtSlot(i).count + 1), i);

            itemFound = true;
            break;
        }
    }

    if (!itemFound) {
        for (int i = 0; i < m_inventory->TOTAL_SLOT_COUNT; i++) {
            if (m_inventory->getItemAtSlot(i).count == 0) {
                m_inventory->addItemToInventory(ItemStack(hitBlock.blockType, 1), i);
                break;
            }
        }
    }
    // Add item to inventory

    // SERVER

    BlockAction blockAction{
        ActionType::BreakBlock, hit.position, hit.chunk->chunkPos, 0, hitBlock.blockType
    };

    m_client.sendPacket(Packet{
        0, PacketType::PACKET_ACTION, Action{ ActionType::BreakBlock, blockAction }
    });
    
    // SERVER

    hitBlock.blockType = BlockType::Air;
}

nearbyChunksType Player::getNeabyChunks(const Vec3& pos, int axisSize)
{
    nearbyChunksType nearbyChunks;

    auto it = world.chunkAt(pos);
    nearbyChunks.emplace_back(it->second.get());

    for (int x = 0; x < axisSize; x++) {
        for (int z = 0; z < axisSize; z++) {

            int dirX = x - std::floor(axisSize / 2);
            int dirZ = z - std::floor(axisSize / 2);
            dirX *= 16; dirZ *= 16;

            if (dirX == 0 && dirZ == 0) {
                continue;
            }

            auto it = world.chunkAt(Vec3(pos.x + dirX, pos.y, pos.z + dirZ));
            if (it == world.allChunks.end()) {
                continue;
            }

            nearbyChunks.emplace_back(it->second.get());
        }
    }

    return nearbyChunks;
}


bool Player::isPlayerOccupyingBlock(const Vec3& playerPos, const Vec3& newObject)
{
    return (Physics::AABBcheckCollisions(
        getHitBox(
            camera->getDataXYZ(),
            AABB(
                Vec3(playerFatness, playerHeight - 0.01, playerFatness),
                Vec3(playerFatness, 0, playerFatness)
            )
        ),
        getHitBox(newObject)
    ));
}

AABB Player::getHitBox(const Vec3& pos, const AABB& size)
{
    return {
        Vec3(
            pos.x - size.min.x,
            pos.y - size.min.y,
            pos.z - size.min.z
        ),
        Vec3(
            pos.x + size.max.x,
            pos.y + size.max.y,
            pos.z + size.max.z
        )
    };
}

void Player::damageEntity(const HitEntity& hitEntity, float damage)
{
    DamageAction dAction{};
    dAction.damage = damage;
    dAction.damagedPlayerID = hitEntity.playerID;

    m_client.sendPacket(Packet{
        0, PacketType::PACKET_ACTION, Action{ ActionType::DamagePlayer, dAction }
    });
}
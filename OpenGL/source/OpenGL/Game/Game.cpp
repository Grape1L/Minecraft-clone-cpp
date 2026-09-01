#include <OpenGL/Game/Game.h>
#include <OpenGL/Window/Window.h>
#include <OpenGL/Graphics/GraphicsEngine.h>
#include <Windows.h>
#include <cmath>
#include <OpenGL/Math/Mat4.h>
#include <OpenGL/Math/Vec3.h>
#include <OpenGL/Math/Vec2.h>
#include <OpenGL/Graphics/VertexArrayObject.h>
#include <OpenGL/Graphics/ShaderProgram.h>
#include <OpenGL/Graphics/UniformBuffer.h>
#include <OpenGL/Entity/EntitySystem.h>
#include <OpenGL/Camera/Camera.h>
#include <OpenGL/Object/Object.h>

#include <OpenGL/World/WorldGen.h>
#include <OpenGL/World/Chunk.h>
#include <thread>
#include <OpenGL/Client/Misc/DataHeader.h>
#include <OpenGL/Client/Misc/InitialData.h>
#include <OpenGL/Client/Misc/ActionHeader.h>
#include <OpenGL/Client/Misc/Packet.h>
#include <OpenGL/Client/Misc/PlayerData.h>
#include <OpenGL/CREATE/Create.h>
#include <OpenGL/Debug.h>
#include <variant>

#include <algorithm>

#include <OpenGL/Blocks/Block.h>


struct UniformData {
    Mat4 world;
    Mat4 projection;
    Mat4 view;
    bool uiMode = false;
};



Game::Game() {
    m_graphicsEngine = std::make_unique<GraphicsEngine>();
    m_display = std::make_unique<Window>();

    m_entitySystem = std::make_unique<EntitySystem>();

    // Server
    m_client = std::make_unique<Client>();
    if (!m_client->connectToServer("127.0.0.1", 2137)) {
        std::cerr << "Failed to connect to server\n";
        return;
    }
    // Server

	m_world = std::make_unique<World>();

    m_player = std::make_unique<Player>(*m_client, *m_world, allPlayersData);
    m_worldGen = std::make_unique<WorldGen>();

    m_display->makeCurrentContext();

    m_graphicsEngine->setViewPort(m_display->getInnerSize());
    m_graphicsEngine->enableDepth();

    m_player->displaySize = m_display->getInnerSize();
}

Game::~Game() {
}

void Game::run() {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    onCreate();

    auto lastTime = std::chrono::high_resolution_clock::now();
    while (running) {
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = (currentTime - lastTime) * 100.0f;
        lastTime = currentTime;
        

        MSG msg = {};
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                continue;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        onUpdateInternal(deltaTime);
    }
    onQuit();
}

void Game::quit() {
    running = false;
}

void Game::onCreate() {

    m_polygonVAO = m_graphicsEngine->createVertexArrayObject(
        {
            (void*)verticesList, 
            sizeof(Vertex), 
            sizeof(verticesList) / sizeof(Vertex), 
            attributesList, 
            sizeof(attributesList) / sizeof(VertexAttribute)
        }, 
        {
            (void*)indicesList, 
            sizeof(indicesList)
        }
    );

    m_uniform = m_graphicsEngine->createUniformBuffer({
        sizeof(UniformData)
    });

    m_shader = m_graphicsEngine->createShaderProgram( 
        {
            L"Assets/Shaders/BasicShader.vert", 
            L"Assets/Shaders/BasicShader.frag"
        } 
    );

    m_shader->setUniformBufferSlot("UniformData", 0);


    // World generation

    // Single player world generation
     m_worldGen->generateFlatWorld(*m_world);



    // Multiplayer world generation

    std::optional<InitialData> initialData;
    
    //while (true) {
    //    initialData = m_client->initialPacketRecv();

    //    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    //    if (initialData != std::nullopt) {
    //        break;
    //    }
    //}
    //m_world->allChunks = initialData.value().allChunks;

    // World generation


    // Server
    std::thread receiveThread([&]() {
        while (true) {
            std::optional<Packet> receivedPacket = std::nullopt;
            receivedPacket = m_client->recvPacket();
            if (receivedPacket == std::nullopt) continue;

            if (receivedPacket.value().packetType == PacketType::PACKET_PLAYER_STATE) {
                PlayerData* playerData = std::get_if<PlayerData>(&receivedPacket.value().packet);
                if (!playerData) continue;

                this->allPlayersData[receivedPacket->senderID] = PlayerData{
                    playerData->playerPos, playerData->yaw, playerData->pitch
                };
            }

            else if (receivedPacket.value().packetType == PacketType::PACKET_ACTION) {
                Action* action = std::get_if<Action>(&receivedPacket.value().packet);
                if (!action) continue;

                if (action->actionType == ActionType::BreakBlock || action->actionType == ActionType::PlaceBlock) {
                    BlockAction* blockAction = std::get_if<BlockAction>(&action->data);

                    allChunksType::iterator it = m_world->allChunks.find(blockAction->chunkPos);
                    if (it == m_world->allChunks.end()) {
                        return;
                    }
                    Chunk& chunk = *it->second;

                    if (blockAction->actionType == ActionType::BreakBlock) {
                        chunk.blocks[(int)blockAction->position.x][(int)blockAction->position.y][(int)blockAction->position.z].blockType == BlockType::Air;
                    }
                    else if (blockAction->actionType == ActionType::PlaceBlock) {

                        chunk.blocks[(int)blockAction->position.x][(int)blockAction->position.y][(int)blockAction->position.z].blockType = blockAction->blockType;
                    }

                    
                }

                else if (action->actionType == ActionType::DamagePlayer) {
                    DamageAction* damageAction = std::get_if<DamageAction>(&action->data);
                    m_player->playerHealth -= damageAction->damage;

                    m_graphicsEngine->clear(Vec4(1, 0, 0, 1));
                }

                m_client->piggyAckPackets.emplace_back(
                    AckPacket{
                        receivedPacket->PiggyAckPacket.id,
                        true
                    }
                );
            }
        }
    });

    receiveThread.detach();
    // Server

    //TEXTURES
    grassBlockTexture.loadTexture("Textures/GrassBlock/grass_block_atlas.png");
    grassBlockTexture.genTexture();

	stoneBlockTexture.loadTexture("Textures/StoneBlock/stone_block_atlas.png");
    stoneBlockTexture.genTexture();

    playerTexture.loadTexture("Textures/OgorekAtlas/atlas.png");
    playerTexture.genTexture();
    //TEXTURES
}



void Game::onUpdateInternal(std::chrono::duration<float> deltaTime) {

    //if (!m_display->isFocused()) {
    //    return;
    //}

    allChunksType::iterator atChunk = m_world->chunkAt(m_player->camera->getDataXYZ());
    if (atChunk != m_world->allChunks.end()) 
    {
        if (chunksToRender.size() == 0 || atChunk->first != chunksToRender[0]->chunkPos)
        {
            chunksToRender = m_player->getNeabyChunks(m_player->camera->getDataXYZ(), 5);
        }
    }

    //std::cout << "FPS: " << m_graphicsEngine->countFPS() << std::endl;

    Mat4 view;
    m_player->onUpdate(view, deltaTime, m_display->isFocused());

    

    auto displaySize = m_display->getInnerSize();
    
    Mat4 projection;

    projection.setPerspectiveRH((float)(90.0f * PI / 180.0f), (float)displaySize.width / (float)displaySize.height, 0.1f, 1000.0f);

    

    m_graphicsEngine->clear(Vec4(0, 1, 1, 1));

    m_graphicsEngine->setFaceCulling(CullType::FrontFace);

    m_graphicsEngine->setWindingOrder(WindingOrder::Clockwise);

    m_graphicsEngine->setVertexArrayObject(m_polygonVAO);

    m_graphicsEngine->setUniformBuffer(m_uniform, 0);

    m_graphicsEngine->setShaderProgram(m_shader);

    
    
    for (const auto& chunk : chunksToRender) {

        for (int x = 0; x < m_world->getChunkSize(); x++) {
            for (int y = 0; y < m_world->getChunkHeight(); y++) {
                for (int z = 0; z < m_world->getChunkSize(); z++) {

                    if (chunk->blocks[x][y][z].blockType == BlockType::Air) {
                        continue;
                    }

                    switch (chunk->blocks[x][y][z].blockType) {
                        case BlockType::Grass:
                            grassBlockTexture.bindTexture();
                            break;
                        case BlockType::Stone:
                            stoneBlockTexture.bindTexture();
                            break;
                        case BlockType::RosieBlock:
                            rosieBlockTexture.bindTexture();
                            break;
                    }

                    Vec3 realPos = Vec3(x + chunk->chunkPos.x, y, z + chunk->chunkPos.y);

                    Object object;
                    Mat4 model = object.modelMatrix(realPos, Vec3(0, 0, 0), Vec3(1, 1, 1));

                    UniformData data = { model, projection, view };
                    m_uniform->setData(&data);

                    m_graphicsEngine->drawIndexedTriangles(TriangleType::TriangleList, 36);
                }
            }
        }
    }


// Rendering Players

    playerTexture.bindTexture();

    for (const auto& [id, playerData] : this->allPlayersData) { // zrobic tez tak zeby nie renderowalo playerow poza render distance
        Object object;
        Mat4 model = object.modelMatrix(playerData.playerPos, Vec3(playerData.pitch, -playerData.yaw - 90, 0), Vec3(0.4, 0.4, 0.4));

        UniformData data = { model, projection, view };
        m_uniform->setData(&data);

        m_graphicsEngine->drawIndexedTriangles(TriangleType::TriangleList, 36);


        model = object.modelMatrix(Vec3(playerData.playerPos.x, playerData.playerPos.y - 0.8, playerData.playerPos.z), Vec3(0, -playerData.yaw - 90, 0), Vec3(0.6, 1.2, 0.35));

        data = { model, projection, view };
        m_uniform->setData(&data);

        m_graphicsEngine->drawIndexedTriangles(TriangleType::TriangleList, 36);
    }

// Rendering Players



    m_display->present(false);
}

void Game::onQuit() {
}

EntitySystem* Game::getEntitySystem() {
    return m_entitySystem.get();
}

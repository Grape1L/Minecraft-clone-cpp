#pragma once

#include <chrono>
#include <OpenGL/MiscStuff.h>
#include <OpenGL/Camera/Camera.h>
#include <vector>
#include <OpenGL/Player/Player.h>
#include <OpenGL/World/WorldGen.h>
#include <OpenGL/Client/Client.h>
#include <OpenGL/Graphics/Textures.h>
#include <unordered_map>
#include <OpenGL/Types.h>
#include <OpenGL/World/World.h>


class Window;
class GraphicsEngine;
class EntitySystem;
class Chunk;

class Camera;
class Player;
class WorldGen;
class World;

class Game {
public:
    Game();
    virtual ~Game();

    void run();
    void quit();

protected:
    virtual void onCreate();
    virtual void onUpdate(float deltaTime) {}
    virtual void onQuit();

private:
    void onUpdateInternal(std::chrono::duration<float> deltaTime);

protected:
    bool running = true;
    std::unique_ptr<Window> m_display;
    std::unique_ptr<GraphicsEngine> m_graphicsEngine;
    std::unique_ptr<EntitySystem> m_entitySystem;


    std::unique_ptr<Player> m_player;
    std::unique_ptr<WorldGen> m_worldGen;

	std::unique_ptr<World> m_world;


    std::unique_ptr<Client> m_client;


    VertexArrayObjectPtr m_polygonVAO;
    ShaderProgramPtr m_shader;
    UniformBufferPtr m_uniform;


    std::chrono::system_clock::time_point m_previousTime;
    float m_scale = 0;

private:
    unsigned int tickRate = 1000;
    double fixedDelta = 1 / tickRate;

private:

    AllPlayersDataType allPlayersData;

    Textures grassBlockTexture{ 0 };
    Textures playerTexture{ 1 };
	Textures stoneBlockTexture{ 2 };

private:
	std::vector<Chunk*> chunksToRender;
};
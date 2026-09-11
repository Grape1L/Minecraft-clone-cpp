# Minecraft Clone in C++ / OpenGL

A Minecraft-inspired voxel game built from scratch in C++ and OpenGL. The project explores real-time 3D rendering, voxel worlds, basic player physics, inventory/item handling, ray casting, and multiplayer networking through a custom UDP networking layer.

> **Status:** Work in progress. This is a first large 3D game project and is not intended to be a finished Minecraft replacement.

## Overview

The project is written primarily in C++20 and uses a small custom OpenGL abstraction rather than a game engine. The codebase is organized around separate systems for rendering, the player, the world/chunks, physics, meshing, and networking.

The current world is chunk-based, with each chunk containing a `16 x 128 x 16` block array. The client generates a flat world locally and renders nearby chunks around the player. Block interaction is performed with ray casting, and player state/actions are exchanged through UDP packets.

## Current Features

- 3D first-person voxel gameplay
- OpenGL 4.6 rendering using a custom rendering layer
- Chunk-based world representation (`16 x 128 x 16` blocks per chunk)
- Flat world generation
- Grass, stone, and air block types
- Texture-mapped blocks using PNG textures and `stb_image`
- First-person mouse look and keyboard movement
- Sprinting and reduced-speed movement
- Gravity and basic AABB-based collision detection
- Jumping
- Ray casting for block and player interaction
- Breaking and placing blocks
- Inventory with hotbar/storage slots
- Item stacks for blocks and weapons
- Basic weapon definitions (wooden sword and pistol)
- Player health and damage interactions
- Multiplayer player-state synchronization
- Custom UDP client communication using Winsock
- Packet types for player state, actions, and chunk initialization
- Acknowledgement packets for received network actions

## Networking

The project uses a custom client/server packet model over UDP rather than a third-party networking library.

The client can send:

- **Player state** — position, yaw, and pitch
- **Block actions** — place and break block requests
- **Damage actions** — player-to-player damage events

Packets are represented with C++ structures and `std::variant`, with separate packet/action headers defining the data transported over the network.

The current client connects to:

```text
127.0.0.1:2137
```

The networking implementation is currently experimental. The repository contains the client-side networking code; the client still owns the local world state, while the planned architecture is to make the server authoritative over the world, physics, and player state.

## Controls

| Input | Action |
|---|---|
| `W A S D` | Move |
| `Mouse` | Look around |
| `Shift` | Move faster / sprint |
| `Ctrl` | Move slower |
| `Space` | Jump |
| `Left Mouse Button` | Break blocks / attack with a weapon |
| `Right Mouse Button` | Place a block |
| `1` - `9` | Select hotbar slot |
| `Backspace` | Reset player position and health |

## Items and Inventory

The inventory currently contains 45 slots: 36 storage slots and 9 hotbar slots.

Items are represented as a `std::variant` so a slot can contain either a block type or a weapon. The current prototype includes:

- Grass blocks
- Stone blocks
- Wooden sword
- Pistol

Weapons have properties such as range, damage, and durability. Block placement consumes one block from the selected inventory slot, while breaking a block adds it to the inventory.

## World and Rendering

### Chunks

The world is stored as an `unordered_map` of chunk coordinates to dynamically allocated `Chunk` objects. Each chunk contains a fixed-size 3D block array:

```text
16 x 128 x 16
```

The client determines a nearby `5 x 5` chunk region around the player and renders the blocks in those chunks.

### Rendering

The renderer is built directly on OpenGL and includes custom wrappers for:

- Vertex array objects
- Index buffers
- Shader programs
- Uniform buffers
- Textures
- Window / OpenGL context management

Shaders are stored in `Assets/Shaders/` and block/player textures are stored under `Textures/`.

### Block Interaction

A ray is cast from the player's camera to detect either a block or another player. The hit result identifies the block position and hit face, allowing blocks to be placed on neighboring faces and allowing players to be targeted by weapons.

## Project Structure

```text
.
├── Assets/
│   └── Shaders/              # GLSL vertex/fragment shaders
├── Game/
│   └── main.cpp              # Application entry point
├── OpenGL/
│   ├── include/OpenGL/       # Public headers
│   │   ├── Blocks/
│   │   ├── Camera/
│   │   ├── Client/
│   │   ├── Graphics/
│   │   ├── Math/
│   │   ├── Meshing/
│   │   ├── Physics/
│   │   ├── Player/
│   │   ├── Weapons/
│   │   ├── Window/
│   │   └── World/
│   ├── source/OpenGL/        # C++ implementations
│   ├── glad/                 # OpenGL loader
│   └── stb/                  # stb_image integration
├── Textures/                 # Block and player textures
└── make_atlas.py             # Texture-atlas helper script
```

## Building

### Requirements

The repository is currently set up as a **Windows / Visual Studio** project.

You will need:

- Windows
- Visual Studio with C++ desktop development tools
- A compiler with C++20 support
- OpenGL support

The Visual Studio project is configured for **C++20** and provides Debug/Release configurations for Win32 and x64. The project links against the Windows OpenGL and Winsock libraries (`opengl32.lib`, `gdi32.lib`, `User32.lib`, and `ws2_32.lib`). The repository also contains the required GLAD and stb-related source/header files.

### Visual Studio

1. Clone the repository.
2. Open `My minecraft clone.slnx` in Visual Studio.
3. Select a Debug or Release configuration.
4. Build the project.
5. Run the resulting executable from a working directory where the relative `Assets/` and `Textures/` paths resolve correctly.

## Development Notes

This project intentionally does not use a full game engine. Rendering, input, world/chunk storage, physics, ray casting, inventory handling, resource management, and the networking protocol are implemented in the repository itself.

The codebase is still evolving, so some systems are prototypes rather than production-ready implementations. In particular, networking, world authority, chunk rendering/meshing, UI, and entity handling are planned areas of further development.

## Roadmap

The following are planned future changes and improvements:

- **Server-authoritative world, players, and physics**  
  Move world simulation and authoritative player/physics state to the server. At the moment, the server-side architecture is primarily acting as a relay and the world can initially differ between players.

- **Item database**  
  Introduce a database describing item categories (blocks, tools, weapons, etc.), durability, and other properties specific to each category.

- **Code refactoring**  
  Refactor and clean up the codebase, especially `Player.cpp` and `Game.cpp`.

- **User interface**  
  Add an in-game UI and improve player interaction/feedback.

- **Chunk meshing**  
  Replace the current per-block rendering approach with more efficient chunk mesh generation.

- **Player bodies**  
  Give players visible bodies instead of the current prototype representation.

- **Tools**  
  Add tools such as pickaxes and axes.

- **Per-block breaking times**  
  Make different block types take different amounts of time to break.

- **Entity system expansion**  
  Extend the entity system to support non-player entities.

- **Unified texture atlas**  
  Combine the block textures into a single PNG atlas instead of maintaining a separate atlas per block.

## Why I Built This

The main goal of this project is to learn by building a complete 3D application from the ground up. It combines several areas of C++ development in one codebase: graphics programming, data structures, memory management, game loops, collision detection, ray casting, input handling, resource management, and network programming.

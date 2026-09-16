#pragma once

#include <variant>
#include <OpenGL/Client/Misc/ActionHeader.h>
#include <OpenGL/Client/Misc/PlayerData.h>
#include <ws2tcpip.h>
#include <OpenGL/Client/Misc/Packet.h>


struct ReceivedPacket {
    sockaddr_in client;
    Packet packet;
};
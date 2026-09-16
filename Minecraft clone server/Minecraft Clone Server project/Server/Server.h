#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <optional>
#include <OpenGL/Client/Misc/ActionHeader.h>
#include <OpenGL/Client/Misc/PlayerData.h>
#include <string>
#include <vector>
#include <OpenGL/World/Chunk.h>
#include "Misc/ReceivedPacket.h"
#include <OpenGL/Client/Misc/Packet.h>
#include <deque>
#include <mutex>
#include <memory>
#include <OpenGL/Types.h>

struct PacketToConfirm {
    Packet packet;
    bool sent = false;
    uint64_t timestamp_us = 0;
};

struct ServerClient {
    ServerClient(int id, const sockaddr_in& c) : clientID(id), client(c) {}

    size_t clientID;
    sockaddr_in client;

    uint64_t lastPing_us = 0;

    std::deque<PacketToConfirm> reliableQueue;
    std::mutex reliableQueueMTX;

    std::deque<Packet> fastQueue;
    std::mutex fastQueueMTX;
};

class Server {
public:
    Server(unsigned short PORT);
    ~Server();

public:
    std::optional<ReceivedPacket> receive();
    void sendPacket(const Packet& packetToSend, const sockaddr_in& client);

public:
    SOCKET listenSock;

    std::vector< std::unique_ptr<ServerClient> > clients;
    std::mutex clientsMutex;

private:
    void acceptClient(const sockaddr_in& client);

private:
    void initialChunksSend(const sockaddr_in& client);
};
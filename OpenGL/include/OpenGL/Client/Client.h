#pragma once

#include <optional>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <OpenGL/Client/Misc/DataHeader.h>
#include <OpenGL/Client/Misc/InitialData.h>
#include <OpenGL/Client/Misc/ActionHeader.h>
#include <OpenGL/Client/Misc/Packet.h>


class Client {
public:
    Client();
    ~Client();

public:
    bool connectToServer(const char* ip, const unsigned short port);

public:
    SOCKET connectSock;

public:
    sockaddr_in server{};
    int serverLen = sizeof(server);

public:
    void sendPacket(const Packet& packetToSend);
    std::optional<Packet> recvPacket();

public:
    std::vector<AckPacket> piggyAckPackets;

private:
    sockaddr_in serverReceival{};
    int serverReceivalLen = sizeof(serverReceival);
};
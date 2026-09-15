#include "Server.h"
#include <iostream>
#include <cstring>
#include "../Misc/Packet.h"
#include <OpenGL/Client/Misc/InitChunk.h>
#include <OpenGL/Client/Misc/Command.h>

Server::Server(unsigned short PORT) {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed!\n" << "Error: " << WSAGetLastError() << "\n";
        return;
    }

    listenSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "listenSock creation failed!\n" << "Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n" << "Error: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return;
    }

    std::cout << "UDP server running on port " << PORT << "\n";
}



Server::~Server() {
    closesocket(listenSock);
    WSACleanup();
}


std::optional<ReceivedPacket> Server::receive() {
    sockaddr_in client{};
    int clientAddrLen = sizeof(client);

    char buffer[1024];
    int bytesRecv = recvfrom(listenSock, buffer, sizeof(buffer), 0, (sockaddr*)&client, &clientAddrLen);
    if (bytesRecv == SOCKET_ERROR) {
        std::cerr << "recvfrom failed: " << WSAGetLastError() << "\n";
        return std::nullopt;
    }

    if (bytesRecv == sizeof(Command)) {
        Command cmd;

        memcpy(&cmd, buffer, sizeof(cmd));
        if (cmd == Command::AcceptMe) {
            this->acceptClient(client);

            return std::nullopt;
        }
    }


    Packet packet;
    memcpy(&packet, buffer, sizeof(packet));

    for (const auto& cptr : clients) {
        std::lock_guard<std::mutex> lock(clientsMutex);

        if (cptr->client.sin_addr.S_un.S_addr == client.sin_addr.S_un.S_addr &&
            cptr->client.sin_port == client.sin_port) {
                packet.senderID = cptr->clientID;
            }
    }

    return ReceivedPacket{client, packet};
}


void Server::sendPacket(const Packet& packetToSend, const sockaddr_in& client) {
    sendto(this->listenSock, (const char*)&packetToSend, sizeof(packetToSend), 0, (sockaddr*)&client, sizeof(client));
}


void Server::acceptClient(const sockaddr_in& client) {
    std::lock_guard<std::mutex> lock(clientsMutex);

    std::cout << "Accepted client: " << client.sin_addr.S_un.S_addr << ":" << client.sin_port << "\n";

    clients.push_back(std::make_unique<ServerClient>(
        clients.size(), client
    ));
}
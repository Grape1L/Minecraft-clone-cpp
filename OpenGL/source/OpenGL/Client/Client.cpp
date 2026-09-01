#include <OpenGL/Client/Client.h>
#include <iostream>
#include <thread>
#include <windows.h>


#include <OpenGL/Client/Misc/PlayerData.h>
#include <OpenGL/Client/Misc/Packet.h>

#include <OpenGL/World/WorldGen.h>

#include <OpenGL/Client/Misc/Command.h>


Client::Client() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    connectSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int timeout = 5000;
    if (setsockopt(connectSock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        std::cout << "setsockopt failed: " << WSAGetLastError() << "\n";
    }
}

Client::~Client() {

}


bool Client::connectToServer(const char* ip, const unsigned short port) {
    server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);
    serverLen = sizeof(server);

    Command cmd = Command::AcceptMe;
    int sendOk = sendto(connectSock, reinterpret_cast<char*>(&cmd), sizeof(cmd), 0, (sockaddr*)&server, serverLen);

    if (sendOk == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << "\n";
        return false;
    }
    
    return true;
}



void Client::sendPacket(const Packet& packetToSend) {
    int sendOk = sendto(connectSock, (const char*)&packetToSend, sizeof(packetToSend), 0, (sockaddr*)&server, serverLen);

    if (sendOk == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << "\n";
    }
}


std::optional<Packet> Client::recvPacket() {
    char buffer[1024];
    int bytesRecv = recvfrom(connectSock, buffer, sizeof(buffer), 0, (sockaddr*)&serverReceival, &serverReceivalLen);

    if (bytesRecv > 0) {
        server = serverReceival;
        serverLen = serverReceivalLen;
    }


    if (bytesRecv == SOCKET_ERROR) {
        return std::nullopt;
    }

    Packet receivedPacket;
    memcpy(&receivedPacket, buffer, sizeof(receivedPacket));

    return receivedPacket;
}
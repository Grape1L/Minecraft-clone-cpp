#include "Server/Server.h"
#include <thread>
#include <memory>
#include <OpenGL/World/World.h>
#include <OpenGL/World/WorldGen.h>
#include <OpenGL/World/Chunk.h>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <optional>


using Clock = std::chrono::steady_clock;

uint64_t now_us() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        Clock::now().time_since_epoch()
    ).count();
}

int main() {
    Server server = Server(2137);

// Gen World
    std::unique_ptr<World> s_world = std::make_unique<World>();
    std::unique_ptr<WorldGen> worldGenPTR = std::make_unique<WorldGen>();

    worldGenPTR->generateFlatWorld(*s_world);
// Gen World


    std::thread recvThread([&]() {
        size_t countIDs = 0;

        while (true) {
            std::optional<ReceivedPacket> receivedPacket = server.receive();

            if (receivedPacket == std::nullopt) {
                continue;
            }

            if (receivedPacket->packet.PiggyAckPacket.ack == true) {

                //std::lock_guard<std::mutex> lock(server.clientsMutex);
                for (auto& clientPTR : server.clients) {
                    ServerClient& client = *clientPTR;

                    if (client.clientID == receivedPacket->packet.senderID) {

                        std::lock_guard<std::mutex> lock(client.reliableQueueMTX);
                        for (size_t i=0; i < client.reliableQueue.size(); i++) {
                            if (client.reliableQueue[i].packet.PiggyAckPacket.id == receivedPacket->packet.PiggyAckPacket.id) {

                                client.lastPing_us = now_us() - client.reliableQueue[i].timestamp_us;

                                client.reliableQueue.erase(client.reliableQueue.begin() + i);

                                break;
                            }
                        }
                    }
                }
            }

            receivedPacket->packet.PiggyAckPacket = {};

            std::lock_guard<std::mutex> lock(server.clientsMutex);
            for (auto& clientPTR : server.clients) {
                ServerClient& client = *clientPTR;

                if (receivedPacket->client.sin_addr.S_un.S_addr == client.client.sin_addr.S_un.S_addr && 
                    receivedPacket->client.sin_port == client.client.sin_port) continue;


                if (receivedPacket->packet.packetType == PacketType::PACKET_ACTION) {
                    Packet copy = receivedPacket->packet;

                    copy.PiggyAckPacket.id = countIDs;

                    std::lock_guard<std::mutex> lock(client.reliableQueueMTX);
                    client.reliableQueue.emplace_back(PacketToConfirm{ copy });

                    countIDs++;
                    if (countIDs > 1000000) countIDs = 0;
                }
                else if (receivedPacket->packet.packetType == PacketType::PACKET_PLAYER_STATE) {
                    std::lock_guard<std::mutex> lock(client.fastQueueMTX);
                    client.fastQueue.emplace_back(receivedPacket->packet);
                }
            }
        }
    });
    recvThread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(15));

    // sendThread
    while (true) {
        //std::lock_guard<std::mutex> lock(server.clientsMutex);
        for (auto& clientPTR : server.clients) {
            ServerClient& client = *clientPTR;

            std::lock_guard<std::mutex> lock(client.reliableQueueMTX);
            for (auto& reliablePacketToSend : client.reliableQueue) {
                if ((!reliablePacketToSend.sent) || (now_us() - reliablePacketToSend.timestamp_us >= client.lastPing_us * 15)) {
                    
                    if (Action* action = std::get_if<Action>(&reliablePacketToSend.packet.packet)) {

                        if (DamageAction* damageAction = std::get_if<DamageAction>(&action->data)) {

                            if (client.clientID == damageAction->damagedPlayerID) {
                                
                                server.sendPacket(reliablePacketToSend.packet, client.client);

                                reliablePacketToSend.sent = true;
                                reliablePacketToSend.timestamp_us = now_us();

                                continue;
                            }
                        }
                    }

                    server.sendPacket(reliablePacketToSend.packet, client.client);
                    
                    reliablePacketToSend.sent = true;
                    reliablePacketToSend.timestamp_us = now_us();
                }
            }

            {
                std::lock_guard<std::mutex> lock(client.fastQueueMTX);

                for (const auto& fastPacketToSend : client.fastQueue) {
                    server.sendPacket(fastPacketToSend, client.client);
                }
                client.fastQueue.clear();
            }


        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    return 0;
}
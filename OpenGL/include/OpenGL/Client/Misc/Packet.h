#pragma once

#include "PacketTypes.h"
#include <variant>
#include "ActionHeader.h"
#include "PlayerData.h"
#include <optional>
#include "AckPacket.h"

using PacketVariant = std::variant<Action, PlayerData, char*>;

struct Packet {
    size_t senderID;
    PacketType packetType;
    PacketVariant packet;

    AckPacket PiggyAckPacket;
};
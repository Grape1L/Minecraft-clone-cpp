#pragma once

#include <cstdint>

enum class PacketType : uint8_t {
    PACKET_PLAYER_STATE, 
    PACKET_ACTION, 
    PACKET_INIT_CHUNKS
};
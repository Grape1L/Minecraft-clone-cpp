#pragma once

#include <variant>
#include "../../Misc/ActionHeader.h"
#include "../../Misc/PlayerData.h"
#include <ws2tcpip.h>
#include "../../Misc/Packet.h"


struct ReceivedPacket {
    sockaddr_in client;
    Packet packet;
};
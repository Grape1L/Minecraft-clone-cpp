#pragma once

struct AckPacket {
    unsigned int id;
    bool ack = false;
};
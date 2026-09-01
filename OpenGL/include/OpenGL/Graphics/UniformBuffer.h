#pragma once

#include <OpenGL/MiscStuff.h>

class UniformBuffer {
public:
    UniformBuffer(const UniformBufferDesc &desc);
    ~UniformBuffer();

    unsigned int getID();

    void setData(void* data);
private:
    unsigned int m_id = 0;
    unsigned int m_size = 0;
};
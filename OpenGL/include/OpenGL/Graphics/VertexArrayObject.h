#pragma once

#include <OpenGL/MiscStuff.h>

class VertexArrayObject {
public:
    VertexArrayObject(const VertexBufferDesc &vbDesc);

    VertexArrayObject(const VertexBufferDesc &vbDesc, const IndexBufferDesc &ibDesc);

    ~VertexArrayObject();

    unsigned int getID();
    uint32_t getVertexBufferSize();
    uint32_t getVertexSize();
private:
    unsigned int m_vertexBufferID = 0;
    unsigned int m_vertexObjectID = 0;
    VertexBufferDesc m_vertexBufferData;
    uint32_t m_elementBufferID = 0;
};
#include <OpenGL/Graphics/VertexArrayObject.h>
#include <glad/glad.h>

VertexArrayObject::VertexArrayObject(const VertexBufferDesc &vbDesc) {
    if (!vbDesc.listSize) DaEngineB_ERROR("VertexArrayObject | listSize is NULL!");
    if (!vbDesc.vertexSize) DaEngineB_ERROR("VertexArrayObject | vertexSize is NULL!");
    if (!vbDesc.verticesList) DaEngineB_ERROR("VertexArrayObject | verticesList is NULL!");

    glGenVertexArrays(1, &m_vertexObjectID);
    glBindVertexArray(m_vertexObjectID);

    glGenBuffers(1, &m_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vbDesc.vertexSize*vbDesc.listSize, vbDesc.verticesList, GL_STATIC_DRAW);

    size_t offsetBytes = 0;
    for (unsigned int i=0; i < vbDesc.attributesListSize; i++) {
        glVertexAttribPointer(
            i, 
            vbDesc.attributesList[i].numElements, 
            GL_FLOAT, 
            GL_FALSE, 
            vbDesc.vertexSize, 
            (void*)offsetBytes
        );
        offsetBytes += (size_t)vbDesc.attributesList[i].numElements * sizeof(float);

        glEnableVertexAttribArray(i);
    }

    m_vertexBufferData = vbDesc;
}

VertexArrayObject::VertexArrayObject(const VertexBufferDesc &vbDesc, const IndexBufferDesc &ibDesc) : VertexArrayObject(vbDesc) {
    if (!ibDesc.listSize) DaEngineB_ERROR("VertexArrayObject | listSize is NULL!");
    if (!ibDesc.indicesList) DaEngineB_ERROR("VertexArrayObject | indicesList is NULL!");

    glBindVertexArray(m_vertexObjectID);

    glGenBuffers(1, &m_elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibDesc.listSize, ibDesc.indicesList, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject() {
    glDeleteBuffers(1, &m_elementBufferID);
    glDeleteBuffers(1, &m_vertexBufferID);
    glDeleteVertexArrays(1, &m_vertexObjectID);
}

unsigned int VertexArrayObject::getID() {
    return m_vertexObjectID;
}

uint32_t VertexArrayObject::getVertexBufferSize() {
    return m_vertexBufferData.listSize;
}

uint32_t VertexArrayObject::getVertexSize() {
    return m_vertexBufferData.vertexSize;
}
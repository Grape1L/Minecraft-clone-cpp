#pragma once

#include <OpenGL/Math/Vec4.h>
#include <OpenGL/Math/Rect.h>
#include <OpenGL/MiscStuff.h>
#include <chrono>

class GraphicsEngine {
public:
    GraphicsEngine();
    ~GraphicsEngine();

    void clear(const Vec4 &color);
    void setViewPort(const Rect &size);
    
    VertexArrayObjectPtr createVertexArrayObject(const VertexBufferDesc &vbDesc);
    VertexArrayObjectPtr createVertexArrayObject(const VertexBufferDesc &vbDesc, const IndexBufferDesc &ibDesc);
    void setVertexArrayObject(const VertexArrayObjectPtr &vao);
    void drawTriangles(const TriangleType &triangleType, unsigned int vertexCount, unsigned int offset);
    void drawIndexedTriangles(const TriangleType &triangleType, unsigned int indicesCount);

    ShaderProgramPtr createShaderProgram(const ShaderProgramDesc &desc);
    void setShaderProgram(const ShaderProgramPtr &program);

    UniformBufferPtr createUniformBuffer(const UniformBufferDesc &desc);
    void setUniformBuffer(UniformBufferPtr &buffer, unsigned int slot);

    void setFaceCulling(const CullType &type);
    void setWindingOrder(const WindingOrder &order);

    void enableDepth();

    int countFPS();

private:
    using clock = std::chrono::high_resolution_clock;
    clock::time_point lastTime = clock::now();
    double elapsedTime = 0;
    int frameCount = 0;
    int fps=0;
};
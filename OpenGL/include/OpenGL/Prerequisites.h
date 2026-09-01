#pragma once
#include <memory>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <random>

constexpr double PI = 3.14159265358979323846;
constexpr double deg2rad = PI / 180.0;

inline double calcRAD(float degrees) {
    return (double)(degrees * deg2rad);
}

inline int randomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution <> distrib(min, max);

    return distrib(gen);
}


class VertexArrayObject;
class ShaderProgram;
class UniformBuffer;

typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
typedef std::shared_ptr<UniformBuffer> UniformBufferPtr;



struct VertexAttribute {
    unsigned int numElements = 0;
};

struct VertexBufferDesc {
    void *verticesList = nullptr;
    unsigned int vertexSize = 0;
    unsigned int listSize = 0;

    VertexAttribute* attributesList = nullptr;
    unsigned int attributesListSize = 0;
};

struct IndexBufferDesc {
    void* indicesList = nullptr;
    unsigned int listSize = 0;

};

struct ShaderProgramDesc {
    const wchar_t* vertexShaderFilePath;
    const wchar_t* fragmentShaderFilePath;
};

struct UniformBufferDesc {
    unsigned int size = 0;
};

enum class ShaderType {
    VertexShader = 0,
    FragmentShader
};

enum class TriangleType {
    TriangleList = 0, 
    TriangleStrip
};

enum class CullType {
    BackFace = 0,
    FrontFace,
    Both
};

enum class WindingOrder {
    Clockwise = 0,
    CounterClockwise
};

#define DaEngineB_ERROR(message) {\
    std::stringstream m;\
    m << "DaEngineB Error: " << message << std::endl;\
    throw std::runtime_error(m.str());\
}

#define DaEngineB_WARNING(message) {\
    std::wclog << "DaEngineB Warning: " << message << std::endl;\
}

#define DaEngineB_INFO(message) {\
    std::wclog << "DaEngineB Info: " << message << std::endl;\
}

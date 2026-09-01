#pragma once

#include <OpenGL/MiscStuff.h>

class ShaderProgram {
public:
    ShaderProgram(const ShaderProgramDesc &desc);

    ~ShaderProgram();

public:
    unsigned int getID();
    void setUniformBufferSlot(const char* name, unsigned int slot);
private:
    void attach(const wchar_t* shaderFilePath, const ShaderType &type);
    void link();

private:
    unsigned int m_programID = 0;
    unsigned int m_attachedShaders[2] = {};
};
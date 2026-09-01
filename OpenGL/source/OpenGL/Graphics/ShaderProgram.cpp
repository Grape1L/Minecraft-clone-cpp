#include <OpenGL/Graphics/ShaderProgram.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

ShaderProgram::ShaderProgram(const ShaderProgramDesc &desc) {
    m_programID = glCreateProgram();
    attach(desc.vertexShaderFilePath, ShaderType::VertexShader);
    attach(desc.fragmentShaderFilePath, ShaderType::FragmentShader);
    link();
}

ShaderProgram::~ShaderProgram() {
    for (unsigned int i=0; i < 2; i++) {
        glDetachShader(m_programID, m_attachedShaders[i]);
        glDeleteShader(m_attachedShaders[i]);
    }

    glDeleteProgram(m_programID);
}

void ShaderProgram::attach(const wchar_t *shaderFilePath, const ShaderType &type) {
    std::string shaderCode;
    std::ifstream shaderStream(shaderFilePath);
    if (!shaderStream.is_open()) {
        DaEngineB_WARNING("ShaderProgram | ShaderProgram::attach " << shaderFilePath << " not found!")
        return;
    }

    std::stringstream sstr;
    sstr << shaderStream.rdbuf();
    shaderCode = sstr.str();
    shaderStream.close();

    unsigned int shaderID = 0;
    if (type == ShaderType::VertexShader) {
        shaderID = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == ShaderType::FragmentShader) {
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    }

    auto sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    int logLenght = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLenght);
    if (logLenght > 0) {
        std::vector<char> errorMessage(logLenght + 1);
        glGetShaderInfoLog(shaderID, logLenght, NULL, &errorMessage[0]);
        DaEngineB_WARNING("ShaderProgram | " << shaderFilePath << " compiled with errors" << std::endl << &errorMessage[0]);
        return;
    }

    glAttachShader(m_programID, shaderID);
    m_attachedShaders[(unsigned int)type] = shaderID;

    DaEngineB_INFO("ShaderProgram | " << shaderFilePath << " compiled successfully");
}
void ShaderProgram::link() {
    glLinkProgram(m_programID);

    int logLenght = 0;
    glGetShaderiv(m_programID, GL_INFO_LOG_LENGTH, &logLenght);
    if (logLenght > 0) {
        std::vector<char> errorMessage(logLenght + 1);
        glGetShaderInfoLog(m_programID, logLenght, NULL, &errorMessage[0]);
        DaEngineB_WARNING("ShaderProgram | " << &errorMessage[0]);
        return;
    }
}

unsigned int ShaderProgram::getID() {
    return m_programID;
}

void ShaderProgram::setUniformBufferSlot(const char* name, unsigned int slot) {
    unsigned int index = glGetUniformBlockIndex(m_programID, name);
    glUniformBlockBinding(m_programID, index, slot);
}
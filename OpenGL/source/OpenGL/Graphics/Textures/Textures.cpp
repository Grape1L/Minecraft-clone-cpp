#include <OpenGL/Graphics/Textures.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <OpenGL/Graphics/ShaderProgram.h>

Textures::Textures(unsigned int textureNum) : texture(textureNum) {
    
}

Textures::~Textures() {
}

void Textures::loadTexture(const char* filename) {
    stbi_set_flip_vertically_on_load(true);
    bytes = stbi_load(filename, &widthImg, &heightImg, &numColorChannel, 0);
    if (!bytes) {
        DaEngineB_ERROR("Texture error || filename not found: " << filename << std::endl);
    }
}

void Textures::genTexture() {
    glGenTextures(1, &texture);
    
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
}

void Textures::free() {
    
}

void Textures::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texture);
}
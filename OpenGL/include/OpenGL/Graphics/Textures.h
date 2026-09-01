#pragma once

#include <OpenGL/MiscStuff.h>

class Textures {
public:
    Textures(unsigned int textureNum);
    ~Textures();
public:
    void loadTexture(const char* filename);
    void genTexture();
    void free();
    void bindTexture();
public:
    int widthImg, heightImg, numColorChannel;
    unsigned int texture;
    unsigned char* bytes = 0;
};
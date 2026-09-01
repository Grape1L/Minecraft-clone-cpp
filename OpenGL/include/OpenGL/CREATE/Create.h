#pragma once

#include <OpenGL/Math/Vec3.h>
#include <OpenGL/Math/Vec2.h>
#include <OpenGL/Math/Mat4.h>

struct Vertex {
    Vec3 position;
    Vec3 colors;
    Vec2 textureCoords;
};
    
Vec3 positionsList[] = {
    // front face
    Vec3(-0.5f, -0.5f, -0.5f),
    Vec3(-0.5f, 0.5f, -0.5f),
    Vec3(0.5f, 0.5f, -0.5f),
    Vec3(0.5f, -0.5f, -0.5f),

    // back face
    Vec3(0.5f, -0.5f, 0.5f),
    Vec3(0.5f, 0.5f, 0.5f),
    Vec3(-0.5f, 0.5f, 0.5f),
    Vec3(-0.5f, -0.5f, 0.5f)
};


const float U1 = 1.0f / 3.0f;
const float U2 = 2.0f / 3.0f;

Vertex verticesList[] = {
    // front face
    { positionsList[0], Vec3(0.2f,0.2f,0.2f), Vec2(0,0) },
    { positionsList[1], Vec3(0.2f,0.2f,0.2f), Vec2(0,1) },
    { positionsList[2], Vec3(0.2f,0.2f,0.2f), Vec2(U1,1) },
    { positionsList[3], Vec3(0.2f,0.2f,0.2f), Vec2(U1,0) },

    // back face
    { positionsList[4], Vec3(0,0,1), Vec2(0,0) },
    { positionsList[5], Vec3(0,1,1), Vec2(0,1) },
    { positionsList[6], Vec3(1,1,0), Vec2(U1,1) },
    { positionsList[7], Vec3(1,0,1), Vec2(U1,0) },

    // top face
    { positionsList[1], Vec3(0,0,1), Vec2(U1,0) },
    { positionsList[6], Vec3(0,1,1), Vec2(U1,1) },
    { positionsList[5], Vec3(1,1,0), Vec2(U2,1) },
    { positionsList[2], Vec3(1,0,1), Vec2(U2,0) },

    // bottom face
    { positionsList[7], Vec3(0,0,1), Vec2(U2,0) },
    { positionsList[0], Vec3(0,1,1), Vec2(U2,1) },
    { positionsList[3], Vec3(1,1,0), Vec2(1,1) },
    { positionsList[4], Vec3(1,0,1), Vec2(1,0) },
    
    // right face
    { positionsList[3], Vec3(0,0,1), Vec2(0,0) },
    { positionsList[2], Vec3(0,1,1), Vec2(0,1) },
    { positionsList[5], Vec3(1,1,0), Vec2(U1,1) },
    { positionsList[4], Vec3(1,0,1), Vec2(U1,0) },

    //left face
    { positionsList[7], Vec3(0,0,1), Vec2(0,0) },
    { positionsList[6], Vec3(0,1,1), Vec2(0,1) },
    { positionsList[1], Vec3(1,1,1), Vec2(U1,1) },
    { positionsList[0], Vec3(1,0,0), Vec2(U1,0) },
};

unsigned int indicesList[] = {
    //front
    0,1,2,
    2,3,0,

    //back
    4,5,6,
    6,7,4,

    //top
    8,9,10,
    10,11,8,

    //bottom
    12,13,14,
    14,15,12,

    //right
    16,17,18,
    18,19,16,

    //left
    20,21,22,
    22,23,20
};

VertexAttribute attributesList[] = {
    sizeof(Vec3) / sizeof(float), // position
    sizeof(Vec3) / sizeof(float), // color
    sizeof(Vec2) / sizeof(float)  // texture coords
};
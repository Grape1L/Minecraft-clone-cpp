#version 410 core

layout (row_major) uniform UniformData {
    mat4 world;
    mat4 projection;
    mat4 view;
    bool uiMode;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colors;
layout(location = 2) in vec2 inTextureCoords;

out vec3 vertOutColor;
out vec2 outTextureCoords;

void main() {
    //if (uiMode == true) {
       // gl_Position = vec4(position.xy, 0.0, 1.0) * projection;
    //} 
    //else {
        gl_Position = vec4(position, 1.0) * world * view * projection;
    //}
    
    vertOutColor = vec3(colors.x, colors.y, colors.z);
    outTextureCoords = inTextureCoords;
}
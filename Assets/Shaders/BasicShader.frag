#version 410 core

in vec3 vertOutColor;
in vec2 outTextureCoords;

layout(location = 0) out vec4 outColor;

uniform sampler2D tex0;

void main() {
    // outColor = vec4(1, 1, 1, 1);

    // outColor.rgb = vertOutColor;

    outColor = texture(tex0, outTextureCoords) * vec4(1, 1, 1, 1.0);
    // outColor = texture(tex0, outTextureCoords);
}
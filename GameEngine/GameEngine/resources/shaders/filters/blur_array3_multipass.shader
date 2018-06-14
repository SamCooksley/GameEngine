#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;

layout (location = 0) out vec2 out_texCoords;

void main()
{
    gl_Position = vec4(in_position, 1.0f); 
    out_texCoords = in_texCoord;
} 

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2DArray tex;

const int SIZE = 4;
const float weight[SIZE] = {
    20.0 / 64.0,
    15.0 / 64.0,
    6.0  / 64.0,
    1.0  / 64.0
};

uniform vec2 scale = vec2(1, 0);
uniform int layer = 0;

void main()
{
    vec2 size = 1.0 / textureSize(tex, 0).xy * scale;
    vec4 result = texture(tex, vec3(in_texCoords, 0)) * weight[0];

    for (int i = 1; i < SIZE; ++i)
    {
        result += texture(tex, vec3(in_texCoords + size * i, 0)) * weight[i];
        result += texture(tex, vec3(in_texCoords - size * i, 0)) * weight[i];
    }

    out_colour = result;//texture(tex, vec3(in_texCoords, gl_Layer));//result;
}
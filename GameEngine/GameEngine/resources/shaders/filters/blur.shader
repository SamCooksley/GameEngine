#include "..\vertex\screen_vert.shader"

#shader fragment
#version 440 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D src;

const float weight[4] = {
    20.0 / 64.0,
    15.0 / 64.0,
    6.0  / 64.0,
    1.0  / 64.0
};

uniform vec2 scale = vec2(1, 0);

void main()
{
    vec2 size = 1.0 / textureSize(src, 0).xy * scale;
    
    vec4 result = texture(src, in_texCoords) * weight[0];

    for (int i = 1; i < 4; ++i)
    {
        result += texture(src, in_texCoords + size * i) * weight[i];
        result += texture(src, in_texCoords - size * i) * weight[i];
    }

    out_colour = result;
}
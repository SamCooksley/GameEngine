#include "..\vertex\screen_vert.glsl"

#shader fragment
#version 440 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D src;

uniform int size = 2;

void main()
{
    vec2 texelSize = 1.0 / textureSize(src, 0).xy;
    vec4 result = vec4(0.0);
    for (int x = -size; x < size; ++x)
    {
        for (int y = -size; y < size; ++y)
        {
            vec2 coords = in_texCoords + vec2(x, y) * texelSize;
            result += texture(src, coords);
        }
    }
    result /= pow(size * 2.0 + 1.0, 2);

    out_colour = result;
}
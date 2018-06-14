#include "..\vertex\screen_vert.shader"

#shader fragment

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D tex;

const int SIZE = 4;
const float weight[SIZE] = {
    20.0 / 64.0,
    15.0 / 64.0,
    6.0  / 64.0,
    1.0  / 64.0
};

uniform vec2 scale = vec2(1, 0);

void main()
{
    vec2 size = 1.0 / textureSize(image, 0);
    vec4 result = texture(tex, in_texCoords) * weight[0];

    for (int i = 1; i < SIZE; ++i)
    {
        result += texture(tex, in_texCoords + size * scale * i) * weight[i];
        result += texture(tex, in_texCoords - size * scale * i) * weight[i];
    }

    out_colour = result;
}
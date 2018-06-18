#blend one one add
#depth none

#include "vertex/screen_vert.shader"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2DArray tex;
uniform int depth = 0;

void main()
{
    out_colour = vec4(vec3(min(texture(tex, vec3(in_texCoords, depth)).r, .3)), 0.7);
}
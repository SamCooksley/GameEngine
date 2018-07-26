#depth none

#include "vertex/screen_vert.shader"

#shader fragment
#version 440 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D tex;
uniform int depth = 1;

void main()
{
    out_colour = vec4(vec3(texture(tex, vec2(in_texCoords)).r), 1);
}
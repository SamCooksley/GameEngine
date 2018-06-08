#blend one one add
#depth none

#include "..\vertex\screen_vert.shader"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D colour;  

uniform vec3 light;

void main()
{
    out_colour = vec4(texture(colour, in_texCoords).rgb * light, 1.0);
}
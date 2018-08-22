#include "..\vertex\screen_vert.glsl"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D tex;

uniform float exposure = 0.5;

void main()
{
    vec3 hdr = texture(tex, in_texCoords).rgb;
    vec3 mapped = 1.0 - exp(-hdr * exposure);
    mapped = pow(mapped, vec3(1.0 / 2.2));

    out_colour = vec4(mapped, 1.0);
}
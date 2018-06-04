#include "screen_vert.shader"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_frag;

#include "camera.shader"
#include "lights.shader"

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D colour;

void main()
{
    vec4 pos = texture(position, in_texCoords);
    vec4 norm = texture(normal, in_texCoords);
    vec4 col = texture(colour, in_texCoords);

    Surface surf;
    surf.colour = col.rgb;
    surf.specular = norm.a;
    surf.shininess = col.a;

    surf.normal = norm.xyz;
    surf.position = pos.xyz;

    vec3 viewDir = normalize(camera.position_world - surf.position);

    vec3 total = lights.ambient * surf.colour;

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        total += CalculateLight(lights.light[i], surf, viewDir);
    }

    out_frag = vec4(total, 1.0); 
}
#blend one one add
#depth none

#include "..\vertex\screen_vert.shader"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D colour;  

#include "..\common\camera.shader"
#include "..\lighting\lighting.shader"
#include "..\lighting\shadows.shader"

uniform DirectionalLight light;

uniform int shadow = 0; 
uniform sampler2D shadowMap;
uniform mat4 lightSpace;
uniform vec3 shadowOrigin;

void main()
{
    vec4 pos  = texture(position, in_texCoords);
    vec4 norm = texture(normal, in_texCoords);
    vec4 col  = texture(colour, in_texCoords);

    Surface surf;
    surf.colour = col.rgb;
    surf.specular = norm.a;
    surf.shininess = col.a;
    surf.normal = norm.xyz;
    surf.position = pos.xyz;

    vec3 viewDir_world = normalize(camera.position_world - surf.position);

    vec3 colour = CalculateLight(light, surf, viewDir_world);
    if (shadow > 0)
    {
        vec3 lightDir = normalize(shadowOrigin - surf.position);
        colour *= ShadowCalculation(lightSpace * vec4(surf.position, 1.0), surf.normal, lightDir, shadowMap);
    }

    out_colour = vec4(colour, 1.0);
}
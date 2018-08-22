#blend one one add
#depth none

#include "..\vertex\screen_vert.glsl"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D colour;  

#include "..\common\camera.glsl"
#include "..\lighting\lighting.glsl"

#define SHADOW_PCF
//#define SHADOW_POISSON_SAMPLE

#include "..\lighting\shadows.glsl"

uniform DirectionalLight light;

uniform int shadow = 0; 
uniform sampler2DShadow shadowMap;
uniform mat4 lightSpace;

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
        colour *= ShadowCalculation(lightSpace * vec4(surf.position, 1.0), light.direction, surf.position, surf.normal, shadowMap);
    }

    out_colour = vec4(colour, 1.0);
}
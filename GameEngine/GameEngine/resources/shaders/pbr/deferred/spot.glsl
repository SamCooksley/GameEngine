#blend one one add
#depth none

#include "..\..\vertex\screen_vert.glsl"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D colour;  

#include "..\..\common\camera.glsl"
#include "..\lighting.glsl"
#include "..\..\lighting\varience_shadows.glsl"

uniform SpotLight light;

uniform int shadow = 0;

uniform mat4 lightSpace;
uniform sampler2D shadowMap;

void main()
{
    Surface surf = SamplersToSurface(in_texCoords, position, normal, colour);

    vec3 viewDir_world = normalize(camera.position_world - surf.position);

    vec3 colour = CalculateLight(light, surf, viewDir_world);

    if (shadow > 0)
    {
        colour *= ShadowCalculation(lightSpace * vec4(surf.position, 1.0), shadowMap);
    }

    out_colour = vec4(colour, 1.0);
}
#blend one one add
#depth none

#include "..\..\vertex\screen_vert.glsl"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

#include "..\..\common\camera.glsl"
#include "..\lighting.glsl"

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D colour;  

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

void main()
{
    Surface surf = SamplersToSurface(in_texCoords, position, normal, colour);

    vec3 viewDir_world = normalize(camera.position_world - surf.position);

    vec3 result = IBL(surf, viewDir_world, irradianceMap, prefilterMap, brdfLUT);
    out_colour = vec4(result, 1.0);
}
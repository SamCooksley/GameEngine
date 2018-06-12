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

//#define SHADOW_PCF
//#define SHADOW_POISSON_SAMPLE
#define DEBUG_CSM

#include "..\lighting\shadows.shader"

uniform DirectionalLight light;

uniform int shadow = 0; 

const int MAX_CASCADES = 3;

uniform int cascadeCount = 3;

uniform sampler2DShadow shadowMap[MAX_CASCADES];
uniform mat4 lightSpace[MAX_CASCADES];
uniform float distance[MAX_CASCADES] = { 10, 17, 25 };

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
        vec4 clip = camera.vp * vec4(surf.position, 1.0);
        float depth = clip.z;

        int index = -1;

        for (int i = 0; i < cascadeCount; ++i)
        {
            if (depth <= distance[i])
            {
                index = i;
                break;
            }
        }

        if (index >= 0)
        {
            colour *= ShadowCalculation(lightSpace[index] * vec4(surf.position, 1.0), light.direction, surf.position, surf.normal, shadowMap[index]);

#ifdef DEBUG_CSM
            const vec3 c[] = {
                vec3(0.5, 0.0, 0.0),
                vec3(0.0, 0.5, 0.0),
                vec3(0.0, 0.0, 0.5)
            };
            colour += c[index];
#endif
        }
    }

    out_colour = vec4(colour, 1.0);
}
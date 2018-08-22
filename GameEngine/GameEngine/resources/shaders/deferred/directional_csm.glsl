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

//#define SHADOW_PCF
//#define SHADOW_POISSON_SAMPLE
//#define DEBUG_CSM

#include "..\lighting\shadows.glsl"

uniform DirectionalLight light;

uniform int shadow = 0; 

uniform int cascadeCount = 1;

uniform sampler2DArray shadowMap;
uniform mat4 lightSpace[10];
uniform float distance[10];

uniform float blurBand = 0.5;

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
        vec4 pos = vec4(surf.position, 1.0);

        vec4 clip = camera.vp * pos;
        float depth = clip.z;

        int index = -1;

        for (int i = 0; i < cascadeCount; ++i)
        {
            if (depth <= distance[i] + blurBand)
            {
                index = i;
                break;
            }
        }
        
        if (index >= 0)
        {
            float shadowFactor = VarienceShadowCalculation(lightSpace[index] * pos, shadowMap, index);

            // fade between cascades.
            if (index < cascadeCount - 1 && depth > distance[index] - blurBand)
            {
                float fade = blurBand + depth - distance[index];
                fade /= blurBand * 2.0;
                fade = clamp(fade / (blurBand * 2.0), 0.0, 1.0) ;
                float shadowFactor2 = VarienceShadowCalculation(lightSpace[index + 1] * pos, shadowMap, index + 1);
                shadowFactor = mix(shadowFactor, shadowFactor2, fade);
            }
            
            colour *= shadowFactor;


#ifdef DEBUG_CSM
            const vec3 c[] = {
                vec3(1, 0, 0),
                vec3(0, 1, 0),
                vec3(0, 0, 1),
            };
            colour += c[index % 3] * 0.05;
#endif
        }
    }

    out_colour = vec4(colour, 1.0);
}
#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoords;

layout (location = 0) out vec2 out_texCoords;

void main()
{
    gl_Position = vec4(in_position, 1.0); 
    out_texCoords = in_texCoords;
} 

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec2 out_colour;

#include "../pbr.glsl"

vec2 IntegrateBRDF(float NdotV, float roughness, int samples)
{
    vec3 N = vec3(0.0, 0.0, 1.0);
    vec3 V = vec3(
        sqrt(1.0 - NdotV * NdotV),
        0.0,
        NdotV
    );

    float A = 0.0;
    float B = 0.0;

    for (int i = 0; i < samples; ++i)
    {
        vec2 Xi = Hammersley(i, samples);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);
        
        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);
        float NdotV = max(V.z, 0.0);
        
        if (NdotL > 0.0)
        {
            float G = GeometrySmithIBL(NdotL, NdotV, roughness);
            float Gvis = (G * VdotH) / (NdotH * NdotV);
            float fc =pow(1.0 - VdotH, 5.0);

            A += (1.0 - fc) * Gvis;
            B += fc * Gvis; 
        }
    }

    A /= float(samples);
    B /= float(samples);

    return vec2(A, B);
}

void main()
{
    out_colour = IntegrateBRDF(in_texCoords.x, in_texCoords.y, 1024);
}
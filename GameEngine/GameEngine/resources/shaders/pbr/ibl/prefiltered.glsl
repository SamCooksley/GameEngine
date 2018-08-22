#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec3 out_position;

uniform mat4 vp;

void main()
{
    gl_Position = vp * vec4(in_position, 1.0);
    out_position = in_position;
}

#shader fragment
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec4 out_colour;

#include "../pbr.glsl"

uniform samplerCube src;
uniform float roughness;

uniform int sampleCount = 1024;

void main()
{
    float res = float(textureSize(src, 0).x);

    vec3 N = normalize(in_position);
    vec3 R = N;
    vec3 V = R;

    float total = 0.0;
    vec3 result = vec3(0.0);

    for (int i = 0; i < sampleCount; ++i)
    {
        vec2 Xi = Hammersley(i, sampleCount);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H  - V);

        float NdotL = max(dot(N, L), 0.0);

        if (NdotL > 0.0)
        {
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float D = DistributionGGX(NdotH, roughness);

            float pdf = D * NdotH / ((4.0 * HdotV) + 0.0001);

            float saTexel = 4.0 * PI / (6.0 * res * res);
            float saSample = 1.0 / (float(sampleCount) * pdf + 0.0001);

            float mipLevel = roughness > 0.0 ? 0.5 * log2(saSample / saTexel) : 0.0;

            result += textureLod(src, L, mipLevel).rgb * NdotL;
            total += NdotL;
        }
    }

    result /= total;
    
    out_colour = vec4(result, 1.0);
}
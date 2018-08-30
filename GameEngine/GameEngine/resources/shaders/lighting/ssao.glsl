#include "..\vertex\screen_vert.glsl"

#shader fragment
#version 430 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out float out_ao;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noise;

uniform int sampleCount = 64;
uniform vec3 samples[64];

uniform float radius = 0.5;
uniform float bias = 0.02;
uniform float intensity = 3.0;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 position = texture(gPosition, in_texCoords).xyz;
    vec3 normal = texture(gNormal, in_texCoords).xyz;
    normal = normalize(normal);

    vec2 noiseScale = textureSize(gPosition, 0).xy / textureSize(noise, 0).xy;
    vec3 randVec = vec3(texture(noise, in_texCoords * noiseScale).xy, 0.0);

    position = (view * vec4(position, 1.0)).xyz;
    normal = mat3(view) * normal;

    float depth = position.z;

    vec3 tangent = randVec - normal * dot(randVec, normal);
    tangent = normalize(tangent);

    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;

    for (int i = 0; i < sampleCount; ++i)
    {
        vec3 s = TBN * samples[i];
        s = position + s * radius;
        
        float depth = s.z;

        vec4 offset = projection * vec4(s, 1.0);
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        vec3 samplePos = texture(gPosition, offset.xy).xyz;
        samplePos = (view * vec4(samplePos, 1.0)).xyz;

        float sampleDepth = samplePos.z;

        float range = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));
        occlusion += step(depth + bias, sampleDepth) * range;
    }

    occlusion = 1.0 - (occlusion / sampleCount);
    occlusion = pow(occlusion, intensity);
    out_ao = occlusion;
}
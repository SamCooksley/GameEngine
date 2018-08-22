#include "constants.glsl"

struct Surface
{
    vec3 albedo;
    float roughness;
    float metallic;
    float ao;

    vec3 F0;

    vec3 normal;
    vec3 position;
};

Surface SamplersToSurface(vec2 _texCoords, sampler2D _position, sampler2D _normal, sampler2D _colour)
{
    vec4 pos  = texture(_position, _texCoords);
    vec4 norm = texture(_normal, _texCoords);
    vec4 col  = texture(_colour, _texCoords);
    
    Surface surf;
    surf.albedo = col.rgb;
    surf.metallic = clamp(norm.a, 0.0, 1.0);
    surf.roughness = clamp(col.a, 0.0, 1.0);
    surf.ao = clamp(pos.a, 0.0, 1.0);
    surf.normal = normalize(norm.xyz);
    surf.position = pos.xyz;
    surf.F0 = mix(DIELECTROC_F0, surf.albedo, surf.metallic);
    return surf;
}
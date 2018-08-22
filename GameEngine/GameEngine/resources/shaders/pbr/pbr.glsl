#include "..\common\constants.glsl"

float DistributionGGX(float _NdotH, float _roughness)
{
    float a2 = _roughness * _roughness;
    a2 *= a2;

    float NdotH2 = _NdotH * _NdotH;

    float num = a2;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float _Ndot, float _roughness)
{
    float r = _roughness + 1.0;
    float k = (r * r) / 8.0;

    float num = _Ndot;
    float denom = _Ndot * (1.0 - k) + k;

    return num / denom; 
}

float GeometrySmith(float _NdotL, float _NdotV, float _roughness)
{
    return GeometrySchlickGGX(_NdotL, _roughness) * GeometrySchlickGGX(_NdotV, _roughness);
}

float GeometrySchlickGGXIBL(float _Ndot, float _roughness)
{
    float a = _roughness;
    float k = (a * a) / 2.0;

    float nom   = _Ndot;
    float denom = _Ndot * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmithIBL(float _NdotL, float _NdotV, float _roughness)
{
    return GeometrySchlickGGXIBL(_NdotL, _roughness) * GeometrySchlickGGXIBL(_NdotV, _roughness);
}

vec3 FresnelSchlick(float _cosTheta, vec3 _F0)
{
    return _F0 + (1.0 - _F0) * pow(1.0 - _cosTheta, 5.0);
}

vec3 FresnelSchlick(float _cosTheta, vec3 _F0, float _roughness)
{
    return _F0 + (max(vec3(1.0 - _roughness), _F0) - _F0) * pow(1.0 - _cosTheta, 5.0);
}

float RadicalInverse_VDC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), RadicalInverse_VDC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
} 
#include "..\common\lighttypes.glsl"
#include "..\common\attenuation.glsl"
#include "surface.glsl"
#include "pbr.glsl"

vec3 CalculateLight(Surface _surface, vec3 _viewDir, vec3 _lightDir, vec3 _radiance)
{   
    vec3 V = _viewDir;
    vec3 L = _lightDir;
    vec3 N = _surface.normal;
    vec3 H = normalize(V + L);
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);

    float NDF = DistributionGGX(NdotH, _surface.roughness);
    float G = GeometrySmith(NdotL, NdotV, _surface.roughness);
    vec3 F = FresnelSchlick(HdotV, _surface.F0);

    vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd *= 1.0 - _surface.metallic;

    vec3 diffuse = kd * _surface.albedo / PI;

    vec3 num = NDF * G * F;
    float denom = 4.0 * NdotV * NdotL;
    vec3 specular = num / max(denom, 0.001);

    return (diffuse + specular) * _radiance * NdotL;
}

vec3 CalculateLight(DirectionalLight _light, Surface _surface, vec3 _viewDir)
{
    return CalculateLight(_surface, _viewDir, _light.direction, _light.colour);
}

vec3 CalculateLight(PointLight _light, Surface _surface, vec3 _viewDir)
{
	vec3 L = _light.position - _surface.position;
	float distance = length(L);
	L /= distance;
    
    float attenuation = CalculateAttenuation(_light.radius, distance);

    vec3 radiance = _light.colour * attenuation;

    return CalculateLight(_surface, _viewDir, L, radiance);
}

vec3 CalculateLight(SpotLight _light, Surface _surface, vec3 _viewDir)
{
	vec3 L = _light.position - _surface.position;
	float distance = length(L);
	L /= distance;
    
    float attenuation = CalculateAttenuation(_light.radius, distance);

    float theta = dot(L, _light.direction);
    float epsilon = _light.cutoff - _light.outerCutoff;
    float intensity = clamp((theta - _light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 radiance = _light.colour * attenuation * intensity;

    return CalculateLight(_surface, _viewDir, L, radiance);
}

vec3 IBL(Surface _surface, vec3 _viewDir, samplerCube _irradianceMap, samplerCube _prefilterMap, sampler2D _brdfLUT)
{
    vec3 N = _surface.normal;
    vec3 V = _viewDir;

    float NdotV = max(dot(N, V), 0.0);

    vec3 R = reflect(-V, N);

    int maxLOD = textureQueryLevels(_prefilterMap);
    vec3 prefitlered = textureLod(_prefilterMap, R, _surface.roughness * float(maxLOD - 1)).rgb;
    
    vec3 F = FresnelSchlick(NdotV, _surface.F0, _surface.roughness);
    vec2 brdf = texture(_brdfLUT, vec2(NdotV, _surface.roughness)).xy;
    vec3 specular = prefitlered * (F * brdf.x + brdf.y);

    vec3 ks = F;
    vec3 kd = 1.0 - ks;
    kd *= 1.0 - _surface.metallic;

    vec3 irradiance = texture(_irradianceMap, N).rgb;

    vec3 diffuse = kd * (irradiance * _surface.albedo);

    return (diffuse + specular) * _surface.ao;
}
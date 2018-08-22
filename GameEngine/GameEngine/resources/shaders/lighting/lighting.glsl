#include "surface.glsl"
#include "..\common\lighttypes.glsl"
#include "..\common\attenuation.glsl"

vec3 CalculateLight(const DirectionalLight _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.direction.xyz);
	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = max(diffuseProduct, 0.0) * pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	return diffuse + specular;
}

vec3 CalculateLight(const PointLight _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = _light.position - _surface.position;
	float distance = length(lightDir);
	lightDir /= distance;

	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = diffuseProduct * pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	return (diffuse + specular) * CalculateAttenuation(_light.radius, distance);
}

vec3 CalculateLight(const SpotLight _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = _light.position - _surface.position;
	float distance = length(lightDir);
	lightDir /= distance;

	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = diffuseProduct * pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	float theta = dot(lightDir, normalize(_light.direction));
	float epsilon = _light.cutoff - _light.outerCutoff;
	float intensity = clamp((theta - _light.outerCutoff) / epsilon, 0.0, 1.0);

	return (diffuse + specular) * CalculateAttenuation(_light.radius, distance) * intensity;
}
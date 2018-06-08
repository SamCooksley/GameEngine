#include "..\common\surface.shader"

struct Attenuation
{
    float linear;
    float quadratic;
};

float CalculateAttenuation(const Attenuation _atten, const vec3 _lightPos, const vec3 _fragPos)
{
	float dist = length(_lightPos - _fragPos);
	return 1.0 / (1.0 + _atten.linear * dist + _atten.quadratic * dist * dist);
}

struct DirectionalLight
{
    vec3 colour;
    vec3 direction;
};

vec3 CalculateLight(const DirectionalLight _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.direction.xyz);
	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = max(diffuseProduct, 0.0) * pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	return diffuse + specular;
}

struct PointLight
{
    vec3 colour;
    vec3 position;

    Attenuation atten;
};

vec3 CalculateLight(const PointLight _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.position.xyz - _surface.position);

	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = max(diffuseProduct, 0.0) * pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	return (diffuse + specular) * CalculateAttenuation(_light.atten, _light.position, _surface.position);
}

struct SpotLight
{
    vec3 colour;

    vec3 position;
    vec3 direction;

    float outerCutoff;
    float cutoff;

    Attenuation atten;
};

vec3 CalculateLight(const SpotLight _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.position.xyz - _surface.position);

	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	float theta = dot(lightDir, normalize(_light.direction.xyz));
	float epsilon = _light.cutoff - _light.outerCutoff;
	float intensity = clamp((theta - _light.outerCutoff) / epsilon, 0.0, 1.0);

	return (diffuse + specular) * CalculateAttenuation(_light.atten, _light.position, _surface.position) * intensity;
}
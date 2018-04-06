#include "surface.shader"

const int LIGHT_NONE = -1;
const int LIGHT_DIRECTIONAL = 0;
const int LIGHT_POINT = 1;
const int LIGHT_SPOT = 2;

struct Light
{
	int type;
	
	float linear;
	float quadratic;
	
	float cutoff;
	float outerCutoff;
	
	vec3 position;
	vec3 direction;
	vec3 colour;
};

vec3 CalculateLight(const Light _light, const Surface _surface, const vec3 _viewDir);

vec3 CalculateDirectionalLight(const Light _light, const Surface _surface, const vec3 _viewDir);
vec3 CalculatePointLight(const Light _light, const Surface _surface, const vec3 _viewDir);
vec3 CalculateSpotLight(const Light _light, const Surface _surface, const vec3 _viewDir);

float CalculateAttenuation(const Light _light, const vec3 _position);

vec3 CalculateLight(const Light _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 light = vec3(0);
	if (_light.type == LIGHT_DIRECTIONAL)
    {
      light = CalculateDirectionalLight(_light, _surface, _viewDir);
    }
    else if (_light.type == LIGHT_POINT)
    {
      light = CalculatePointLight(_light, _surface, _viewDir);
    }
    else if (_light.type == LIGHT_SPOT)
    {
      light = CalculateSpotLight(_light, _surface, _viewDir);
    }
	return light;
}

vec3 CalculateDirectionalLight(const Light _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.direction.xyz);
	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = max(diffuseProduct, 0.0) * pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	return diffuse + specular;
}

vec3 CalculatePointLight(const Light _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.position.xyz - _surface.position);

	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = max(diffuseProduct, 0.0) * pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	return (diffuse + specular) * CalculateAttenuation(_light, _surface.position);
}

vec3 CalculateSpotLight(const Light _light, const Surface _surface, const vec3 _viewDir)
{
	vec3 lightDir = normalize(_light.position.xyz - _surface.position);

	float diffuseProduct = max(dot(_surface.normal, lightDir), 0.0);
	vec3 diffuse = diffuseProduct * (_light.colour.rgb * _surface.colour);

	vec3 halfDir = normalize(lightDir + _viewDir);
	vec3 specular = pow(max(dot(_surface.normal, halfDir), 0.0), _surface.shininess) * (_light.colour.rgb * _surface.specular);

	float theta = dot(lightDir, normalize(_light.direction.xyz));
	float epsilon = _light.cutoff - _light.outerCutoff;
	float intensity = clamp((theta - _light.outerCutoff) / epsilon, 0.0, 1.0);

	return (diffuse + specular) * CalculateAttenuation(_light, _surface.position) * intensity;
}

float CalculateAttenuation(const Light _light, const vec3 _position)
{
	float dist = length(_light.position.xyz - _position);
	return 1.0 / (1.0 + _light.linear * dist + _light.quadratic * dist * dist);
}
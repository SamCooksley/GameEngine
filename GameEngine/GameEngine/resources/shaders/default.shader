#shader vertex
#version 430 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_texCoords;
layout (location = 2) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
	vec3 position_world;
	vec2 texCoords;
	vec3 normal;
} vs_out;

void main()
{
	vec4 world = model * in_position;
	vs_out.position_world = world.xyz;
	gl_Position = projection * view * world;
	vs_out.texCoords = in_texCoords;
	
	mat3 m3 = mat3(transpose(inverse(model)));
	
	vs_out.normal = normalize(m3 * in_normal);
}

#shader fragment
#version 430 core

in VS_OUT
{
	vec3 position_world;
	vec2 texCoords;
	vec3 normal;
} fs_in;

layout (location = 0) out vec4 out_frag;

uniform sampler2D diffuse;
uniform float specular = 1;
uniform float shininess = 1;

uniform vec3 view_position_world;

struct Surface
{
	vec3 colour;
	float specular;
	float shininess;
	
	vec3 normal;
	vec3 position;
};

const int MAX_LIGHTS = 10;

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

uniform Light lights[MAX_LIGHTS];
uniform vec3 ambient = vec3(0.1);

vec3 CalculateLight(const Light _light, const Surface _surface, const vec3 _viewDir);

vec3 CalculateDirectionalLight(const Light _light, const Surface _surface, const vec3 _viewDir);
vec3 CalculatePointLight(const Light _light, const Surface _surface, const vec3 _viewDir);
vec3 CalculateSpotLight(const Light _light, const Surface _surface, const vec3 _viewDir);

float CalculateAttenuation(const Light _light, const vec3 _position);

void main()
{
	vec3 viewDir = normalize(view_position_world - fs_in.position_world);

	Surface surf;
	surf.position = fs_in.position_world;
	surf.normal = fs_in.normal;

	surf.colour = texture(diffuse, fs_in.texCoords).rgb;
	surf.specular = specular;
	surf.shininess = shininess;

	vec3 total = ambient * surf.colour;

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		total += CalculateLight(lights[i], surf, viewDir);
	}

	out_frag = vec4(total, 1.0);
}

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
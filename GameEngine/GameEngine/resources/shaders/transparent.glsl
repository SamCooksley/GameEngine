#queue transparent
#blend srcalpha oneminussrcalpha add
#cull none

#include "vertex/default.glsl"

#shader fragment
#version 430 core

in VS_OUT
{
	vec3 position_world;
	vec2 texCoords;
	mat3 tbn;

	vec3 normal;

	vec3 position_tan;
	vec3 view_position_tan;
} fs_in;

layout (location = 0) out vec4 out_frag;

#include "lighting/lighting.glsl"
#include "common/lights.glsl"
#include "common/parallax.glsl"

#include "common/camera.glsl"

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specular;

uniform float shininess = 2;

uniform sampler2D height;
uniform float heightScale = 0.1;

void main()
{
	vec3 viewDir_world = normalize(camera.position_world - fs_in.position_world);

	vec3 viewDir_tan = normalize(fs_in.view_position_tan - fs_in.position_tan);

	vec2 texCoords = ParallaxMapping(fs_in.texCoords, viewDir_tan, heightScale, height);

	Surface surf;
	surf.position = fs_in.position_world;

	surf.normal = texture(normal, texCoords).rgb;
	surf.normal = surf.normal * 2.0 - 1.0;
	surf.normal = normalize(fs_in.tbn * surf.normal);

	vec4 colour = texture(diffuse, texCoords).rgba;
	surf.colour = colour.rgb;
	surf.specular = texture(specular, texCoords).r;
	surf.shininess = shininess;

	vec3 total = lights.ambient.rgb * surf.colour;

	for (int i = 0; i < lights.numDirectional; ++i)
	{
		total += CalculateLight(lights.directional[i], surf, viewDir_world);
	}

	for (int i = 0; i < lights.numPoint; ++i)
	{
		total += CalculateLight(lights.point[i], surf, viewDir_world);
	}

	for (int i = 0; i < lights.numSpot; ++i)
	{
		total += CalculateLight(lights.spot[i], surf, viewDir_world);
	}

	out_frag = vec4(total, colour.a);
}
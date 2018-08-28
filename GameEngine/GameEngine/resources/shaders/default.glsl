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

#include "lighting/lights.glsl"
#include "common/parallax.glsl"

#include "common/camera.glsl"

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specular;
uniform sampler2D opacity;
uniform sampler2D height;

uniform float heightScale = 0.1;
uniform float shininess = 2;

void main()
{
	vec3 viewDir_world = normalize(camera.position_world - fs_in.position_world);

	vec3 viewDir_tan = normalize(fs_in.view_position_tan - fs_in.position_tan);

	vec2 texCoords = ParallaxMapping(fs_in.texCoords, viewDir_tan, heightScale, height);


	if (texCoords.x < 0.0 || texCoords.x > 1.0 ||
	texCoords.y < 0.0 || texCoords.y > 1.0)
	{
		//discard;
	}

	if (texture(opacity, texCoords).r < 0.1)
	{
		discard;
	}

	Surface surf;
	surf.position = fs_in.position_world;

	surf.normal = texture(normal, texCoords).rgb;
	surf.normal = surf.normal * 2.0 - 1.0;
	surf.normal = normalize(fs_in.tbn * surf.normal);

	surf.colour = texture(diffuse, texCoords).rgb;
	surf.specular = texture(specular, texCoords).r;
	surf.shininess = shininess;

	vec3 total = lights.ambient * surf.colour;

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

	out_frag = vec4(total, 1.0);
}
#queue transparent
#blend srcalpha oneminussrcalpha add

#include "../vertex/default.glsl"

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

#include "../common/parallax.glsl"

#include "../common/camera.glsl"

#include "lighting.glsl"
#include "../common/lights.glsl"

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D height;
uniform sampler2D metallic;
uniform sampler2D roughness;
uniform sampler2D ao;

uniform float heightScale = 0.1;

void main()
{
	vec3 viewDir_world = normalize(camera.position_world - fs_in.position_world);

	vec3 viewDir_tan = normalize(fs_in.view_position_tan - fs_in.position_tan);

	vec2 texCoords = ParallaxMapping(fs_in.texCoords, viewDir_tan, heightScale, height).xy;

    vec4 colour = texture(albedo, texCoords);
    float alpha = colour.a;

	if (alpha < 0.00001)
	{
		discard;
	}

	Surface surf;
    surf.albedo = colour.rgb;
    surf.roughness = texture(roughness, texCoords).r;
    surf.metallic = texture(metallic, texCoords).r;
    surf.ao = texture(ao, texCoords).r;

    surf.F0 = mix(DIELECTROC_F0, surf.albedo, surf.metallic);

    surf.normal = texture(normal, texCoords).rgb;
    surf.normal = surf.normal * 2.0 - 1.0;
    surf.normal = normalize(fs_in.tbn * surf.normal);

    surf.position = fs_in.position_world;

	vec3 total = surf.albedo * lights.ambient.rgb * surf.ao;

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

	out_frag = vec4(total, alpha);
}
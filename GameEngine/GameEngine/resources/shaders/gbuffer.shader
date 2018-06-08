#queue deferred

#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoords;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_bitangent;

out VS_OUT
{
	vec3 position_world;
	vec2 texCoords;
	mat3 tbn;

	vec3 normal;

	vec3 position_tan;
	vec3 view_position_tan;
} vs_out;

#include "common/camera.shader"

uniform mat4 model;

void main()
{
    vec4 world = model * vec4(in_position, 1.0);
	vs_out.position_world = world.xyz;
	gl_Position = camera.vp * world;
	vs_out.texCoords = in_texCoords;
	
	mat3 m3 = mat3(transpose(inverse(model)));

	vec3 t = normalize(m3 * in_tangent);
	vec3 b = normalize(m3 * in_bitangent);
	vec3 n = normalize(m3 * in_normal);

	vs_out.tbn = mat3(t, b, n);
	vs_out.normal = n;

	mat3 tbn = transpose(vs_out.tbn);

	vs_out.position_tan      = tbn * vs_out.position_world;
	vs_out.view_position_tan = tbn * camera.position_world;
}

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

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_colour;

#include "common/parallax.shader"
#include "common/camera.shader"

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specular;
uniform sampler2D opacity;
uniform sampler2D displacement;

uniform float displacementScale = 0.1;
uniform float shininess = 2;

void main()
{
    vec3 viewDir_tan = normalize(fs_in.view_position_tan - fs_in.position_tan);
	vec2 texCoords = ParallaxMapping(fs_in.texCoords, viewDir_tan, displacementScale, displacement);

    if (texture(opacity, texCoords).r < 0.1)
	{
		discard;
	}

    out_position.xyz = fs_in.position_world;
    out_position.w = 0.0;

    vec3 norm = texture(normal, texCoords).rgb;
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.tbn * norm);

    out_normal.xyz = norm;
    out_normal.w = texture(specular, texCoords).r;

    out_colour.rgb = texture(diffuse, texCoords).rgb;
    out_colour.a = shininess;
}
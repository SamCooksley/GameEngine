#shader vertex
#version 430 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_texCoords;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_bitangent;

out VS_OUT
{
	vec3 position_world;
	vec2 texCoords;
	mat3 tbn;

	vec3 position_tan;
	vec3 view_position_tan;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 view_position_world;

void main()
{
	vec4 world = model * in_position;
	vs_out.position_world = world.xyz;
	gl_Position = projection * view * world;
	vs_out.texCoords = in_texCoords;
	
	mat3 m3 = mat3(transpose(inverse(model)));

	vec3 t = normalize(m3 * in_tangent);
	vec3 b = normalize(m3 * in_bitangent);
	vec3 n = normalize(m3 * in_normal);
	
	vs_out.tbn = mat3(t, b, n);

	mat3 tbn = transpose(vs_out.tbn);

	vs_out.position_tan      = tbn * vs_out.position_world;
	vs_out.view_position_tan = tbn * view_position_world;
}

#shader fragment
#version 430 core

in VS_OUT
{
	vec3 position_world;
	vec2 texCoords;
	mat3 tbn;

	vec3 position_tan;
	vec3 view_position_tan;
} fs_in;

layout (location = 0) out vec4 out_frag;

#include "lighting.shader"
#include "parallax.shader"

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specular;
uniform float shininess = 2;

uniform sampler2D displacement;
uniform float displacementScale = 0.1;

uniform vec3 view_position_world;

uniform Light lights[MAX_LIGHTS];
uniform vec3 ambient = vec3(0.1);

void main()
{
	vec3 viewDir_world = normalize(view_position_world - fs_in.position_world);

	vec3 viewDir_tan = normalize(fs_in.view_position_tan - fs_in.position_tan);
	vec2 texCoords = ParallaxMapping(fs_in.texCoords, viewDir_tan, displacementScale, displacement);

	Surface surf;
	surf.position = fs_in.position_world;

	surf.normal = texture(normal, texCoords).rgb;
	surf.normal = normalize(surf.normal * 2.0 - 1.0);
	surf.normal = normalize(fs_in.tbn * surf.normal);

	surf.colour = texture(diffuse, texCoords).rgb;
	surf.specular = texture(specular, texCoords).r;
	surf.shininess = shininess;

	vec3 total = ambient * surf.colour;

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		total += CalculateLight(lights[i], surf, viewDir_world);
	}

	out_frag = vec4(total, 1.0);
}
#shader vertex
#version 430 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
	vec2 texCoords;
} vs_out;

void main()
{
	gl_Position = projection * view * model * in_position;
	vs_out.texCoords = in_texCoords;
}

#shader fragment
#version 430 core

layout (location = 0) out vec4 out_frag;

uniform sampler2D diffuse;

in VS_OUT
{
	vec2 texCoords;
} fs_in;

void main()
{
	out_frag = texture(diffuse, fs_in.texCoords);
}
#shader vertex
#version 430 core

layout (location = 0) in vec4 in_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * in_position;
}

#shader fragment
#version 430 core

out vec4 out_frag;

void main()
{
	out_frag = vec4(1.0, 0.0, 1.0, 1.0);
}
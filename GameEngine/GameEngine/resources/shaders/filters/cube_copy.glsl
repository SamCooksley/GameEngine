#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec3 out_position;

uniform mat4 vp;

void main()
{
    gl_Position = vp * vec4(in_position, 1.0);
    out_position = in_position;
}

#shader fragment
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec4 out_colour;

uniform samplerCube src;

void main()
{
    out_colour = texture(src, in_position);
}
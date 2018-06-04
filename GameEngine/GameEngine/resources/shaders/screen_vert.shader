#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;

layout(location = 0) out vec2 out_texCoords;

void main()
{
    gl_Position = vec4(in_position, 1.0f); 
    out_texCoords = in_texCoord;
} 
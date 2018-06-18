#cull none

#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(in_position, 1.0);
}

#shader geometry 
#version 430 core

layout (invocations = 4) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

const int MAX_CASCADES = 5;

uniform int cascadeCount;
uniform mat4 vp[MAX_CASCADES];

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Layer = gl_InvocationID;
        gl_Position = vp[gl_InvocationID] * gl_in[i].gl_Position; 
        EmitVertex();
    }
    EndPrimitive();
}

#shader fragment
#version 430 core

void main()
{ }
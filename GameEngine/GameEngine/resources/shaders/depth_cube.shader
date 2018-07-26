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

layout (invocations = 6) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (location = 0) out vec3 out_position;

uniform mat4 vp[6];

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Layer = gl_InvocationID;
        gl_Position = vp[gl_InvocationID] * gl_in[i].gl_Position; 
        out_position = gl_in[i].gl_Position.xyz;
        EmitVertex();
    }
    EndPrimitive();
}   

#shader fragment
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec2 out_moment;

uniform vec3 position;
uniform float near;
uniform float far;

void main()
{ 
    float depth = length(in_position - position);
    depth = (depth - near) / (far - near);
    gl_FragDepth = depth;

    float dx = dFdx(depth);
    float dy = dFdy(depth);
    float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);
    out_moment = vec2(depth, moment2);
}
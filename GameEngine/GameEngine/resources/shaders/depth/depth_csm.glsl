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

layout (triangles) in;
layout (triangle_strip, max_vertices = 30) out;

uniform int cascadeCount = 1;
uniform mat4 vp[10];

void main()
{
    int layers = min(cascadeCount, 10);
    for (int layer = 0; layer < layers; ++layer)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Layer = layer;
            gl_Position = vp[layer] * gl_in[i].gl_Position; 
            EmitVertex();
        }
        EndPrimitive();
    }
}   

#shader fragment
#version 430 core

layout (location = 0) out vec2 out_moment;

void main()
{ 
    float depth = gl_FragCoord.z;
    float dx = dFdx(depth);
    float dy = dFdx(depth);
    float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);
    out_moment = vec2(depth, moment2);
}
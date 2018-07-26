#cull none

#shader vertex
#version 440 core

layout (location = 0) in vec3 in_position;

uniform mat4 model;
uniform mat4 view;

uniform vec3 origin;

void main()
{
    gl_Position = model * vec4(in_position, 1.0);    
    gl_Position = vec4(origin - gl_Position.xyz, gl_Position.w);     
}

#shader geometry 
#version 440 core

layout (invocations = 2) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (location = 0) out float out_depth;

uniform float near;
uniform float far;

void main()
{
    float dir = gl_InvocationID * 2 - 1; // 0 = -1, 1 = 1

    for (int i = 0; i < 3; ++i)
    {
        gl_Layer = gl_InvocationID;

        gl_Position = gl_in[i].gl_Position;
        
        gl_Position.z *= dir;

        float s = sign(sign(gl_Position.z) + 0.5);
        
        float l = length(gl_Position.xyz);
        gl_Position /= l;

        gl_Position.z = abs(gl_Position.z) + 1.0;
        gl_Position.x /= gl_Position.z;
        gl_Position.y /= gl_Position.z;

        float depth_dir = (l * s - near) / (far - near);

        out_depth = abs(depth_dir);  

        gl_Position.z = depth_dir * 2.0 - 1.0;
        gl_Position.w = 1.0;

        EmitVertex();
    }
    EndPrimitive();
}   

#shader fragment
#version 440 core

layout (location = 0) in float in_depth;
layout (location = 1) in float in_clip;

layout (location = 0) out vec2 out_moment;

void main()
{
    gl_FragDepth = in_depth;
    float depth = in_depth;

    float dx = dFdx(depth);
    float dy = dFdy(depth);
    float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);
    out_moment = vec2(depth, moment2);
}
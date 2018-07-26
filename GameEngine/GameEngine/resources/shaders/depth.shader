#cull none

#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;

uniform mat4 vp;
uniform mat4 model;

void main()
{
    gl_Position = vp * model * vec4(in_position, 1.0);
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
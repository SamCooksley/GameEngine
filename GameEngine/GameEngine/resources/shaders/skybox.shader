#shader vertex
#version 430 core

layout (location = 0) in vec4 in_position;

out VS_OUT
{
    vec3 texCoords;
} vs_out;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = (projection * mat4(mat3(view)) * in_position).xyww;
    vs_out.texCoords = in_position.xyz;
}

#shader fragment
#version 430 core

in VS_OUT
{
    vec3 texCoords;
} fs_in;

uniform samplerCube cubemap;

layout (location = 0) out vec4 out_colour;

void main()
{
    out_colour = texture(cubemap, fs_in.texCoords);
}
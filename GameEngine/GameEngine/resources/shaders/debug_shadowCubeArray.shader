#depth always

#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;

out VS_OUT
{
    vec3 texCoords;
} vs_out;

#include "common/camera.shader"

void main()
{
    gl_Position = (camera.projection * mat4(mat3(camera.view)) * vec4(in_position, 1.0)).xyww;
    vs_out.texCoords = in_position.xyz;
}

#shader fragment
#version 430 core

in VS_OUT
{
    vec3 texCoords;
} fs_in;

uniform sampler2DArray tex;

layout (location = 0) out vec4 out_colour;

#include "sample.shader"

void main()
{
    out_colour = vec4(vec3(texture(tex, CubeCoords(fs_in.texCoords)).r), 1);
}
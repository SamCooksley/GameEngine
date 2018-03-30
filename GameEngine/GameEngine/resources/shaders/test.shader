#shader vertex
#version 330

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec3 in_normal;

#include "includeTest.shader"

out VS_OUT
{
  vec3 position_world;
  vec2 texCoords;
  vec3 normal;
} vs_out;

void main()
{
  vec4 worldPos = model * in_position;
  gl_Position = projection * view * worldPos;
  vs_out.position_world = worldPos.xyz;

  vs_out.texCoords = in_texCoord;

  vs_out.normal = in_normal;
}

#shader fragment
#version 330

in VS_OUT
{
  vec3 position_world;
  vec2 texCoords;
  vec3 normal;
} vs_out;

layout(location = 0) out vec4 out_colour;

void main()
{
  out_colour = vec4(1, 0, 0, 1);
}
layout (std140) uniform Camera
{
  mat4 projection;
  mat4 view;
  mat4 vp;
  vec3 position_world;
} camera;
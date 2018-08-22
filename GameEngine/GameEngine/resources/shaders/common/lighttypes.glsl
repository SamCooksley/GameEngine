struct DirectionalLight
{
  vec3 colour;
  vec3 direction;
};

struct PointLight
{
  float radius;

  vec3 colour;
  vec3 position;
};

struct SpotLight
{
  float radius;

  float cutoff;
  float outerCutoff;

  vec3 colour;

  vec3 position;
  vec3 direction;
};
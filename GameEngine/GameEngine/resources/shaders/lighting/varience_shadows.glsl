#include "../common/functions.glsl"
#include "../common/dualparaboloid.glsl"

float ShadowCalculation(const vec4 _position_light, const sampler2D _shadowMap)
{
  vec3 coords = _position_light.xyz / _position_light.w;
	coords = coords * 0.5 + 0.5;

  vec2 moments = texture(_shadowMap, coords.xy).xy;
  float p = step(coords.z, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.0002);

  float d  = coords.z - moments.x;
  float pMax = InverseMix(0.1, 1.0, variance / (variance + d * d));

  return max(p, pMax);
}

float ShadowCalculation(const vec4 _position_light, const sampler2DArray _shadowMap, int _depth)
{
  vec3 coords = _position_light.xyz / _position_light.w;
  coords = coords * 0.5 + 0.5;

  vec2 moments = texture(_shadowMap, vec3(coords.xy, _depth)).xy;

  float variance = max(moments.y - moments.x * moments.x, 0.0001);

  float d  = coords.z - moments.x;
  float p = InverseMix(0.8, 1.0, variance / (variance + d * d));

  return max(p, step(coords.z, moments.x));
}

float ShadowCalculation(const vec3 _lightPos, const float _near, const float _far, const vec3 _fragPos, const samplerCube _shadowMap)
{
  vec3 lightDir = _fragPos - _lightPos;
  float depth = length(lightDir);
  depth = (depth - _near) / (_far - _near);
  depth = min(depth, 1.0);

  vec2 moments = texture(_shadowMap, lightDir).xy;

  float p = step(depth, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.0001);

  float d = depth - moments.x;
  float pMax = InverseMix(0.2, 1.0, variance / (variance + d * d));

  return max(p, pMax);
}

float ShadowCalculation(const vec3 _fragPos, const float _near, const float _far, const sampler2DArray _shadowMap)
{
  float l = length(_fragPos);
  vec3 dir = (_fragPos / l);

  float depth = (l - _near) / (_far - _near);
  depth = min(depth, 1.0);  

  vec3 coords = DualParaboloidTexCoords(dir);

  vec2 moments = texture(_shadowMap, coords).xy;

  float p = step(depth, moments.x);

  float variance = max(moments.y - moments.x * moments.x, 0.0001);

  float d = depth - moments.x;
  float pMax = InverseMix(0.2, 1.0, variance / (variance + d * d));

  return max(p, pMax);
}
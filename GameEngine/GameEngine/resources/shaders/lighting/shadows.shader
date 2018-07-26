#include "..\common\surface.shader"

float ShadowCalculation(const vec4 _lightPos, const vec3 _lightDir, const vec3 _fragPos, const vec3 _normal, const sampler2D _shadowMap)
{
  vec3 coords = _lightPos.xyz / _lightPos.w;
  coords = coords * 0.5 + 0.5;
  coords.z -= 0.001 + 0.004 * (1.0 - abs(dot(_normal, _lightDir)));

  float shadow = 0.0;

  vec2 texelSize = 1.0 / textureSize(_shadowMap, 0);
  const int range = 4;

  for (int y = -range; y <= range; ++y)
  {
  	for (int x = -range; x <= range; ++x)
  	{
  		shadow += step(coords.z, texture(_shadowMap, coords.xy + vec2(x, y) * texelSize).r);
  	}
  }

  shadow /= (range * 2.0 + 1.0) * (range * 2.0 + 1.0);

  if (coords.z > 1.0)
  {
      shadow = 1.0;
  }

  return shadow;
}

float ShadowCalculation(const vec4 _lightPos, const vec3 _lightDir, const vec3 _fragPos, const vec3 _normal, const sampler2DArray _shadowMap, int _depth)
{
  vec3 coords = _lightPos.xyz / _lightPos.w;
  coords = coords * 0.5 + 0.5;
  coords.z -= 0.001 + 0.004 * (1.0 - abs(dot(_normal, _lightDir)));

  return step(coords.z, texture(_shadowMap, vec3(coords.xy, _depth)).r);
  float shadow = 0.0;

  vec2 texelSize = 1.0 / textureSize(_shadowMap, 0).xy;
  const int range = 4;

  for (int y = -range; y <= range; ++y)
  {
  	for (int x = -range; x <= range; ++x)
  	{
  		shadow += step(coords.z, texture(_shadowMap, vec3(coords.xy + vec2(x, y) * texelSize, _depth)).r);
  	}
  }

  shadow /= (range * 2.0 + 1.0) * (range * 2.0 + 1.0);

  if (coords.z > 1.0)
  {
      shadow = 1.0;
  }

  return shadow;
}

float ShadowCalculation(const vec4 _lightPos, const vec3 _lightDir, const vec3 _fragPos, const vec3 _normal, const sampler2DShadow _shadowMap)
{
	vec3 coords = _lightPos.xyz / _lightPos.w;
	coords = coords * 0.5 + 0.5;
  coords.z -= 0.0025 + 0.01 * (1.0 - abs(dot(_normal, _lightDir))); 
  
  float shadow = 0.0;

  vec2 texelSize = 1.0 / textureSize(_shadowMap, 0);
  const int range = 3;

  for (int y = -range; y <= range; ++y)
  {
  	for (int x = -range; x <= range; ++x)
  	{
  		shadow += texture(_shadowMap, vec3(coords.xy + vec2(x, y) * texelSize, coords.z));
  	}
  }

  shadow /= (range * 2.0 + 1.0) * (range * 2.0 + 1.0);

  if (coords.z > 1.0)
  {
      shadow = 1.0;
  }

  return shadow;
}

float ShadowCalculation(const vec4 _lightPos, const vec3 _lightDir, const vec3 _fragPos, const vec3 _normal, const sampler2DArrayShadow _shadowMap, int _depth)
{
	vec3 coords = _lightPos.xyz / _lightPos.w;
	coords = coords * 0.5 + 0.5;
  coords.z -= 0.0025 + 0.01 * (1.0 - abs(dot(_normal, _lightDir))); 
  
  float shadow = 0.0;

  vec2 texelSize = 1.0 / textureSize(_shadowMap, 0).xy;
  const int range = 1;

  for (int y = -range; y <= range; ++y)
  {
  	for (int x = -range; x <= range; ++x)
  	{
  		shadow += texture(_shadowMap, vec4(coords.xy + vec2(x, y) * texelSize, _depth, coords.z));
  	}
  }

  shadow /= (range * 2.0 + 1.0) * (range * 2.0 + 1.0);

  if (coords.z > 1.0)
  {
      shadow = 1.0;
  }

  return shadow;
}

float ShadowCalculation(const vec3 _lightPos, const float _near, const float _far, const vec3 _fragPos, const samplerCube _shadowMap)
{
  vec3 lightDir = _fragPos - _lightPos;
  float depth = length(lightDir);
  depth = (depth - _near) / (_far - _near);
  depth -= 0.001;
  depth = min(depth, 1.0);

  float closest = texture(_shadowMap, lightDir).r;

  return step(depth, closest);
}

float ShadowCalculation(const vec3 _lightPos, const float _near, const float _far, const vec3 _fragPos, const vec3 _normal, const samplerCubeShadow _shadowMap)
{
  vec3 lightDir = _fragPos - _lightPos;
  float depth = length(lightDir);
  depth = (depth - _near) / (_far - _near);
  depth -= 0.001 + 0.01 * (1.0 - abs(dot(_normal, normalize(lightDir))));
  depth = min(depth, 1.0);
  
  return texture(_shadowMap, vec4(lightDir, depth)).r;
}

float inverseMix(float _a, float _b, float _value)
{
  return (_value - _a) / (_b - _a);
}

float inverseMix01(float _a, float _b, float _value)
{
  return clamp(inverseMix(_a, _b, _value), 0.0, 1.0);
}

float VarienceShadowCalculation(const vec4 _lightPos, const sampler2D _shadowMap)
{
  vec3 coords = _lightPos.xyz / _lightPos.w;
	coords = coords * 0.5 + 0.5;

  vec2 moments = texture(_shadowMap, coords.xy).xy;
  float p = step(coords.z, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.0002);

  float d  = coords.z - moments.x;
  float pMax = inverseMix01(0.1, 1.0, variance / (variance + d * d));

  return max(p, pMax);
}

float VarienceShadowCalculation(const vec4 _lightPos, const sampler2DArray _shadowMap, int _depth)
{
  vec3 coords = _lightPos.xyz / _lightPos.w;
	coords = coords * 0.5 + 0.5;

  vec2 moments = texture(_shadowMap, vec3(coords.xy, _depth)).xy;

  float variance = max(moments.y - moments.x * moments.x, 0.0001);

  float d  = coords.z - moments.x;
  float p = inverseMix01(0.8, 1.0, variance / (variance + d * d));

  return max(p, step(coords.z, moments.x));
}

#include "..\sample.shader"

float VarienceShadowCalculation(const vec3 _lightPos, const float _near, const float _far, const vec3 _fragPos, const samplerCube _shadowMap)
{
  vec3 lightDir = _fragPos - _lightPos;
  float depth = length(lightDir);
  depth = (depth - _near) / (_far - _near);
  depth = min(depth, 1.0);

  vec2 moments = texture(_shadowMap, lightDir).xy;

  float p = step(depth, moments.x);
  float variance = max(moments.y - moments.x * moments.x, 0.0001);

  float d = depth - moments.x;
  float pMax = inverseMix01(0.2, 1.0, variance / (variance + d * d));

  return max(p, pMax);
}

#include "..\dp.shader"

float VarienceShadowCalculation(const vec3 _frag_light, const float _near, const float _far, const sampler2DArray _shadowMap)
{
  float l = length(_frag_light);
  vec3 dir = (_frag_light / l);

  float depth = (l - _near) / (_far - _near);
  // continue shadows past far.
  

  vec3 coords = DualParaboloidTexCoords(dir);

  vec2 moments = texture(_shadowMap, coords).xy;

  float p = step(depth, moments.x);

  float variance = max(moments.y - moments.x * moments.x, 0.0001);

  float d = depth - moments.x;
  float pMax = inverseMix01(0.2, 1.0, variance / (variance + d * d));

  return max(p, pMax);
}
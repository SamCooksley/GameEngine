float ShadowCalculation(const vec4 _positionLight, const vec3 _normal, const vec3 _lightDir, const sampler2D _shadowMap)
{
  vec3 coords = _positionLight.xyz / _positionLight.w;
  coords = coords * 0.5 + 0.5;
  coords.z += -mix(0.001, 0.0005, clamp(dot(_normal, _lightDir), 0, 1));

  vec2 texelSize = 1.0 / textureSize(_shadowMap, 0);

  float shadow = 0.0;
  const int range = 2;

  for (float y = -range; y <= range; ++y)
  {
  	for (float x = -range; x <= range; ++x)
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
#ifdef SHADOW_POISSON_SAMPLE
  const int POISSON_DISK_SIZE = 16;

  const vec2 POISSON_DISK[POISSON_DISK_SIZE] = vec2[](
     vec2(-0.94201624, -0.39906216), 
     vec2( 0.94558609, -0.76890725), 
     vec2(-0.09418410, -0.92938870), 
     vec2( 0.34495938,  0.29387760), 
     vec2(-0.91588581,  0.45771432), 
     vec2(-0.81544232, -0.87912464), 
     vec2(-0.38277543,  0.27676845), 
     vec2( 0.97484398,  0.75648379), 
     vec2( 0.44323325, -0.97511554), 
     vec2( 0.53742981, -0.47373420), 
     vec2(-0.26496911, -0.41893023), 
     vec2( 0.79197514,  0.19090188), 
     vec2(-0.24188840,  0.99706507), 
     vec2(-0.81409955,  0.91437590), 
     vec2( 0.19984126,  0.78641367), 
     vec2( 0.14383161, -0.14100790) 
  );
#endif

float Random(const vec4 _seed)
{
  float dotProd = dot(_seed, vec4(12.9898,78.233,45.164,94.673));
  return fract(sin(dotProd) * 43758.5453);
}

float ShadowCalculation(const vec4 _lightPos, const vec3 _lightDir, const vec3 _fragPos, const vec3 _normal, const sampler2D _shadowMap)
{
  vec3 coords = _lightPos.xyz / _lightPos.w;
  coords = coords * 0.5 + 0.5;
  coords.z -= 0.001 + 0.004 * (1.0 - abs(dot(_normal, _lightDir)));

  float shadow = 0.0;

#ifdef SHADOW_PCF

#ifdef SHADOW_POISSON_SAMPLE

  const int samples = 4;

  for (int i = 0; i < samples; ++i)
  {
    int index = int(POISSON_DISK_SIZE * Random(vec4(floor(_fragPos * 1000.0), i))) % POISSON_DISK_SIZE;
    shadow += step(coords.z, texture(_shadowMap, coords.xy + POISSON_DISK[index] * 0.001).z);
  }

  shadow /= samples;

#else

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

#endif

#else

  shadow += step(coords.z, texture(_shadowMap, coords.xy).r);

#endif

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

#ifdef SHADOW_PCF

#ifdef SHADOW_POISSON_SAMPLE

  const int samples = 4;

  for (int i = 0; i < samples; ++i)
  {
    int index = int(POISSON_DISK_SIZE * Random(vec4(floor(_fragPos * 1000.0), i))) % POISSON_DISK_SIZE;
    shadow += texture(_shadowMap, vec3(coords.xy + POISSON_DISK[index] * 0.001, coords.z));
  }

  shadow /= samples;

#else

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

#endif

#else

  shadow = texture(_shadowMap, coords);

#endif

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

#ifdef SHADOW_PCF

#ifdef SHADOW_POISSON_SAMPLE

  const int samples = 4;

  for (int i = 0; i < samples; ++i)
  {
    int index = int(POISSON_DISK_SIZE * Random(vec4(floor(_fragPos * 1000.0), i))) % POISSON_DISK_SIZE;
    shadow += texture(_shadowMap, vec4(coords.xy + POISSON_DISK[index] * 0.001, _depth, coords.z));
  }

  shadow /= samples;

#else

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

#endif

#else

  shadow = texture(_shadowMap, vec4(coords.xy, _depth, coords.z));

#endif

  if (coords.z > 1.0)
  {
      shadow = 1.0;
  }

  return shadow;
}
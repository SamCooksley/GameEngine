vec3 ParallaxMapping(const vec2 _texCoords, const vec3 _viewDir, const float _scale, const sampler2D _parallax)
{ 
    const float minLayers = 8.0;
	const float maxLayers = 64.0;
	float t = clamp(abs(dot(vec3(0.0, 0.0, 1.0), _viewDir)), 0.0, 1.0);
	float layerCount = mix(maxLayers, minLayers, t);  
    
    float layerStep = 1.0 / layerCount;
    float curHeight = 1.0;
	
	vec2 P = _viewDir.xy / _viewDir.z * _scale;
    vec2 deltaTexCoords =  P / layerCount;

    vec2  curTexCoords = _texCoords;
	float curMapHeight = texture(_parallax, curTexCoords).r;
  
	while(curHeight > curMapHeight)
	{
	    curTexCoords -= deltaTexCoords;
	    curMapHeight = texture(_parallax, curTexCoords).r;  
	    curHeight -= layerStep;  
	}

	vec2 prevTexCoords = curTexCoords + deltaTexCoords;

	float afterHeightDelta  = curMapHeight - curHeight;
	float beforeHeightDelta = texture(_parallax, prevTexCoords).r - curHeight - layerStep;

	float weight = afterHeightDelta / (afterHeightDelta - beforeHeightDelta);
	vec2 texCoords = mix(curTexCoords, prevTexCoords, weight);
    float height = curHeight + mix(beforeHeightDelta, afterHeightDelta, weight);

	return vec3(texCoords, height);  
} 
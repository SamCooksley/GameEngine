vec2 ParallaxMapping(const vec2 _texCoords, const vec3 _viewDir, const float _scale, const sampler2D _parallax)
{ 
    const float minLayers = 64.0;
	const float maxLayers = 256.0;
	float t = clamp(abs(dot(vec3(0.0, 0.0, 1.0), _viewDir)), 0.0, 1.0);
	float layerCount = mix(maxLayers, minLayers, t);  
    
    float layerDepth = 1.0 / layerCount;
    float currentDepth = 0.0;
	
    vec2 P = _viewDir.xy * _scale; 
    vec2 deltaTexCoords = P / layerCount;

    vec2  texCoords = _texCoords;
	float currentMapDepth = texture(_parallax, texCoords).r;
	float prevMapDepth = currentMapDepth;
  
	while(currentDepth < currentMapDepth)
	{
	    texCoords -= deltaTexCoords;
		prevMapDepth = currentMapDepth;
	    currentMapDepth = texture(_parallax, texCoords).r;  
	    currentDepth += layerDepth;  
	}

	vec2 prevTexCoords = texCoords + deltaTexCoords;
	
	float afterDepth  = currentMapDepth - currentDepth;
	float beforeDepth = prevMapDepth - currentDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	//texCoords = mix(texCoords, prevTexCoords, weight);
	texCoords = prevTexCoords * weight + texCoords * (1.0 - weight);

	return texCoords;  
}  
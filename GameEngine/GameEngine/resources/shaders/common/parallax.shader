vec2 ParallaxMapping(const vec2 _texCoords, const vec3 _viewDir, const float _scale, const sampler2D _parallax)
{ 
    const float minLayers = 64.0;
	const float maxLayers = 256.0;
	float t = clamp(abs(dot(vec3(0.0, 0.0, 1.0), _viewDir)), 0.0, 1.0);
	float layerCount = mix(maxLayers, minLayers, t);  
    
    float layerDepth = 1.0 / layerCount;
    float currentDepth = 0.0;
	
    vec2 deltaTexCoords =  (_viewDir.xy * _scale) / layerCount;

    vec2  texCoords = _texCoords;
	float currentMapDepth = 1.0 - texture(_parallax, texCoords).r;
  
	while(currentDepth < currentMapDepth)
	{
	    texCoords -= deltaTexCoords;
	    currentMapDepth = 1.0 - texture(_parallax, texCoords).r;  
	    currentDepth += layerDepth;  
	}

	vec2 prevTexCoords = texCoords + deltaTexCoords;

	float afterDepth  = currentMapDepth - currentDepth;
	float beforeDepth = (1.0 - texture(_parallax, prevTexCoords).r) - currentDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	texCoords = mix(texCoords, prevTexCoords, weight);

	return texCoords;  
}  
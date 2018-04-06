#include "lighting.shader"

const int MAX_LIGHTS = 10;

layout (std140) uniform Lights
{ 
	Light light[MAX_LIGHTS];
	vec3 ambient;
} lights;
#include "lighting.shader"

const int MAX_DIRECTIONAL = 5;
const int MAX_POINT = 10;
const int MAX_SPOT = 5;

//could be more efficiently packed, however, this increases ease of use.
layout (std140) uniform Lights
{
    int numDirectional;
    int numPoint;
    int numSpot;
    vec3 ambient;
    DirectionalLight directional[MAX_DIRECTIONAL];
    PointLight point[MAX_POINT];
    SpotLight spot[MAX_SPOT];
} lights;
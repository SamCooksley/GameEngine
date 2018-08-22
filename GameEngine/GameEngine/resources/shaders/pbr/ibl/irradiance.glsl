#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec3 out_position;

uniform mat4 vp;

void main()
{
    gl_Position = vp * vec4(in_position, 1.0);
    out_position = in_position;
}

#shader fragment
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec4 out_colour;

#include "..\..\common\constants.glsl"

uniform samplerCube src;

void main()
{
    vec3 N = normalize(in_position);

    vec3 up = vec3 (0.0, 1.0, 0.0);
    vec3 right = cross(up, N);
    up = cross(N, right);

    vec3 irradiance = vec3(0.0);

    float sampleDelta = 0.025;
    int numSamples = 0;

    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tangent = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 ssample = tangent.x * right + tangent.y * up + tangent.z * N;

            irradiance += texture(src, ssample).rgb * cos(theta) * sin(theta);
            ++numSamples;
        }
    }

    irradiance = (PI * irradiance) / float(numSamples);

    out_colour = vec4(irradiance, 1.0);
}
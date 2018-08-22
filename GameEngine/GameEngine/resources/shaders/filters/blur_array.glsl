#include "../vertex/screen_vert.glsl"

#shader geometry
#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 30) out; // max 10 layers

layout (location = 0) in vec2 in_texCoords[3];

layout (location = 0) out vec2 out_texCoords;

uniform int layerCount = 1;

void main()
{
    int layers = min(layerCount, 10);
    for (int layer = 0; layer < layers; ++layer)
    {
        for (int i = 0; i < 3; ++i)
        {
            gl_Layer = layer;
            gl_Position = gl_in[i].gl_Position; 
            out_texCoords = in_texCoords[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}

#shader fragment
#version 440 core

layout (location = 0) in vec2 in_texCoords;
layout (location = 1) flat in int in_layer;

layout (location = 0) out vec4 out_colour;

uniform sampler2DArray src;

const float weight[4] = {
    20.0 / 64.0,
    15.0 / 64.0,
    6.0  / 64.0,
    1.0  / 64.0 
};

uniform vec2 scale = vec2(1, 0);

void main()
{
    vec2 size = 1.0 / textureSize(src, 0).xy * scale;
    
    vec4 result = texture(src, vec3(in_texCoords, gl_Layer)) * weight[0];

    for (int i = 1; i < 4; ++i)
    {
        result += texture(src, vec3(in_texCoords + size * i, gl_Layer)) * weight[i];
        result += texture(src, vec3(in_texCoords - size * i, gl_Layer)) * weight[i];
    }

    out_colour = result;
}
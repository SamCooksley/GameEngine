#include "../vertex/screen_vert.shader"

#shader geometry
#version 440 core

layout (invocations = 4) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 9) out;

layout (location = 0) in vec2 in_texCoords[3];

layout (location = 0) out vec2 out_texCoords;
layout (location = 1) flat out int out_layer; // use manual layer (not gl_Layer in fragment) for increased compatibility.

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Layer = gl_InvocationID;
        gl_Position = gl_in[i].gl_Position; 
        out_texCoords = in_texCoords[i];
        out_layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}

#shader fragment
#version 440 core

layout (location = 0) in vec2 in_texCoords;
layout (location = 1) flat in int in_layer;

layout (location = 0) out vec4 out_colour;

uniform sampler2DArray tex;

const float weight[4] = {
    20.0 / 64.0,
    15.0 / 64.0,
    6.0  / 64.0,
    1.0  / 64.0
};

uniform vec2 scale = vec2(1, 0);

uniform vec2 layerMultiplier[4] = { 
    vec2(1.0), vec2(1.0), vec2(1.0), vec2(1.0)
};

void main()
{
    int layer = in_layer; // gl_Layer; 
    vec2 size = 1.0 / textureSize(tex, 0).xy * scale * layerMultiplier[layer];
    
    vec4 result = texture(tex, vec3(in_texCoords, layer)) * weight[0];

    for (int i = 1; i < 4; ++i)
    {
        result += texture(tex, vec3(in_texCoords + size * i, layer)) * weight[i];
        result += texture(tex, vec3(in_texCoords - size * i, layer)) * weight[i];
    }

    out_colour = result;
}
#shader vertex
#version 440 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texCoord;

layout (location = 0) out vec2 out_texCoords;

void main()
{
    gl_Position = vec4(in_position, 1.0f); 
    out_texCoords = in_texCoord;
} 

#shader geometry
#version 440 core

layout (invocations = 3) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 9) out;

layout (location = 0) in vec2 in_texCoords[3];

layout (location = 0) out vec2 out_texCoords;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Layer = gl_InvocationID;
        gl_Position = gl_in[i].gl_Position; 
        out_texCoords = in_texCoords[i];
        EmitVertex();
    }
    EndPrimitive();
}

#shader fragment
#version 440 core

layout (location = 0) in vec2 in_texCoords;

layout (location = 0) out vec4 out_colour;

uniform sampler2DArray tex;

const float weight[4] = {
    20.0 / 64.0,
    15.0 / 64.0,
    6.0  / 64.0,
    1.0  / 64.0
};

uniform vec2 scale = vec2(1, 0);
uniform float layerMultiplier[3] = { 1.0, 1.0, 1.0 };

void main()
{
    int layer = gl_Layer;
    vec2 size = 1.0 / textureSize(tex, 0).xy * scale * layerMultiplier[layer];


//out_colour = texture(tex, vec3(in_texCoords, 0));

    vec4 result = texture(tex, vec3(in_texCoords, layer)) * weight[0];
    //out_colour = vec4(in_texCoords, 0.0, 1.0);
    //return;
    for (int i = 1; i < 4; ++i)
    {
        result += texture(tex, vec3(in_texCoords + size * i, layer)) * weight[i];
        result += texture(tex, vec3(in_texCoords - size * i, layer)) * weight[i];
    }

    out_colour = result;
}
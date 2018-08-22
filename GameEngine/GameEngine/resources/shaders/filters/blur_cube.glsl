#shader vertex
#version 430 core

layout (location = 0) in vec3 in_position;

layout (location = 0) out vec3 out_texCoords;

void main()
{
    gl_Position = (camera.projection * mat4(mat3(camera.view)) * vec4(in_position, 1.0)).xyww;
    vs_out.texCoords = in_position.xyz;
}

#shader geometry
#version 440 core

layout (invocations = 6) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out; // max 10 layers

layout (location = 0) in vec2 in_texCoords[3];

layout (location = 0) out vec2 out_texCoords;

uniform int layerCount = 1;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Layer = gl_InvocationID;
        gl_Position = gl_in[i].gl_Position; 
        EmitVertex();
    }
    EndPrimitive();
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
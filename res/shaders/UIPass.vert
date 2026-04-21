#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 uv; 

uniform vec2 offsets[64];
uniform vec2 sizes[64];

void main()
{
    vec2 position = offsets[gl_InstanceID]; 
    vec2 size = sizes[gl_InstanceID];
    uv = aTexCoords;

    gl_Position = vec4(aPos.xy * size + position, 0.0, 1.0);
}


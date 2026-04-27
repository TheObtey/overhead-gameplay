#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 uv; 
flat out int id;
flat out int instanceId;

uniform vec2 resolution;

uniform vec2 offsets[64];
uniform vec2 sizes[64];
uniform vec2 uvPos[64][4];
uniform vec2 uvSize[64];
uniform int textureId[64];


void main()
{
    vec2 pixelPos = aPos.xy * sizes[gl_InstanceID];
    pixelPos += offsets[gl_InstanceID];

    vec2 normalizedPos = (2.0 * pixelPos - resolution) / resolution;
    normalizedPos.y *= -1.0;

    uv = uvPos[gl_InstanceID][gl_VertexID];
    id = textureId[gl_InstanceID];
    instanceId = gl_InstanceID;

    gl_Position = vec4(normalizedPos, 0.0, 1.0);
}


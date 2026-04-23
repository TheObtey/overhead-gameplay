#version 450
layout (location = 0) out vec4 fragColor;

in vec2 uv;
flat in int instanceId;
flat in int id;

uniform bool isText[64];
uniform vec4 tint[64];
uniform sampler2D textures[16];

void main()
{
    if (isText[id] == true)
        fragColor = tint[id] * vec4(1.0f, 1.0f, 1.0f, texture(textures[id], uv).r);

    //fragColor = tint * vec4(1.0f, 1.0f, 1.0f, texture(textures[id], uv).r);
    else
        fragColor = tint[id] * texture(textures[id], uv);
}


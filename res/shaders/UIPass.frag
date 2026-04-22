#version 450
layout (location = 0) out vec4 fragColor;

in vec2 uv;
flat in int id;

uniform sampler2D textures[16];

void main()
{
    fragColor = texture(textures[id], uv);
}


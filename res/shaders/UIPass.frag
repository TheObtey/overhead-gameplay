#version 450
layout (location = 0) out vec4 fragColor;

in vec2 uv;

uniform sampler2D textures[64];

void main()
{
    fragColor = texture(textures[0], uv);
}


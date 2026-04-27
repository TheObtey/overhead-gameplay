#version 450 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_opacity1;

void main()
{    
    gPosition = vec4(FragPos, 1.0f);

    //gNormal = normalize(Normal);
    gNormal = vec4(normalize(texture(texture_normal1, TexCoords).rgb * Normal * 2.0 - 1.0), 1.0);
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;

    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r * texture(texture_opacity1, TexCoords).r;
}

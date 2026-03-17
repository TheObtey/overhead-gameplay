#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (location = 3) out vec3 FragPos;
layout (location = 4) out vec3 Normal;
layout (location = 5) out vec2 TexCoords;

layout (location = 6) uniform mat4 model;
layout (location = 7) uniform mat4 view;
layout (location = 8) uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz; 
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;

    gl_Position = projection * view * worldPos;
}

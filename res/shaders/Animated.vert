layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec3 aWeights;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

const int MAX_BONES = 200;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 gBones[MAX_BONES];

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);

    mat4 BoneTransform = gBones[aBoneIDs[0]] * aWeights[0];
    BoneTransform += gBones[aBoneIDs[1]] * aWeights[1];
    BoneTransform += gBones[aBoneIDs[2]] * aWeights[2];
    BoneTransform += gBones[aBoneIDs[3]] * aWeights[3];

    vec4 PosL = BoneTransform * vec4(aPosition, 1.0);
    gl_Position = projection * worldPos * view * PosL;
    TexCoord = aTexCoord;
    vec4 NormalL = BoneTransform * vec4(aNormal, 0.0);
    Normal = (gWorld * NormalL).xyz;
    FragPos = (gWorld * PosL).xyz;
}

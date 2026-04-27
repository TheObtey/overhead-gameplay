#include "AssetLoading/FBXLoader.h"
#include "Servers/GraphicServer.h"
#include "AssetLoading/AssetLoader.h"

#include <fstream>
#include <Mesh.h>
#include <Logger.hpp>
#include <MathUtils.h>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace
{
    bool IsNearlyIdentity(glm::mat4 const& m, float eps = 1e-4f)
    {
        glm::mat4 const id(1.0f);
        for (int c = 0; c < 4; ++c)
        {
            for (int r = 0; r < 4; ++r)
            {
                if (std::abs(m[c][r] - id[c][r]) > eps) return false;
            }
        }
        return true;
    }

    glm::mat4 ExtractTopLevelConverter(aiNode const* root, auto&& toGlm)
    {
        glm::mat4 converter(1.0f);
        aiNode const* n = root;

        while (n)
        {
            bool const converterNode = (n->mNumMeshes == 0) && (n->mNumChildren == 1);
            if (!converterNode) break;

            glm::mat4 const local = toGlm(n->mTransformation);
            if (!IsNearlyIdentity(local))
            {
                converter = converter * local;
            }

            n = n->mChildren[0];
        }

        return converter;
    }
}


uint8 FBXLoader::m_sTexTypes[] = {
        aiTextureType_DIFFUSE,
        aiTextureType_SPECULAR,
        aiTextureType_NORMALS,
        aiTextureType_HEIGHT,
        aiTextureType_OPACITY,
        aiTextureType_EMISSIVE,
        aiTextureType_AMBIENT,
};

glm::mat4x4 AIMatrixToGLMMatrix(aiMatrix4x4 const& matrix)
{
    glm::mat4x4 out = {};
    memcpy(&out, &matrix, sizeof(aiMatrix4x4));
    return glm::transpose(out);
}

std::string FBXLoader::NormalizeTexturePath(std::string const& path)
{
    std::filesystem::path p(path);
    return p.lexically_normal().generic_string();
}


void FBXLoader::LoadTextures(FBXLoader::Material& materials, std::vector<sptr<Ore::Texture>>& vect, uint32 matIndex)
{
    for (std::map<Ore::TextureMaterialType, std::string>::iterator it = materials.textures[matIndex].begin(); it != materials.textures[matIndex].end(); ++it)
    {
        sptr<Ore::Texture> texture = AssetLoader::GetSharedTexture(it->second, it->first);
        if (texture)
            vect.push_back(texture);
    }
}

void FBXLoader::LoadDefaultsTextures(SceneMesh& mesh)
{
    sptr<Ore::Texture> text = AssetLoader::GetSharedTexture(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::NORMAL), Ore::TextureMaterialType::NORMAL);
    if (text)
        mesh.paths.push_back(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::NORMAL));
    mesh.meshTextures.push_back(text);

    sptr<Ore::Texture> text2 = AssetLoader::GetSharedTexture(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::DIFFUSE), Ore::TextureMaterialType::DIFFUSE);
    if (text2)
        mesh.paths.push_back(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::DIFFUSE));
    mesh.meshTextures.push_back(text);

    sptr<Ore::Texture> text3 = AssetLoader::GetSharedTexture(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::SPECULAR), Ore::TextureMaterialType::SPECULAR);
    if (text3)
        mesh.paths.push_back(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::SPECULAR));
    mesh.meshTextures.push_back(text);
}

sptr<SceneData> FBXLoader::LoadFile(std::string const& path)
{
    Assimp::Importer importer = Assimp::Importer();
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    int importFlags = aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_GlobalScale;

    aiScene const* pAScene = importer.ReadFile(path.c_str(), importFlags);
    if (pAScene == nullptr)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Failed Loading " + path + " file");
        return nullptr;
    }

    SceneData uScene = {};
    uScene.path = path;
    Material allTextures;
    std::map<std::string, glm::mat4> bonesTransforms;
    uint32 nodeCount = 0;
    BuildMaterials(pAScene, allTextures);
    BuildNodesTree(pAScene, pAScene->mRootNode, -1, uScene, allTextures, bonesTransforms);
    BuildMeshs(pAScene, uScene, allTextures, bonesTransforms);

    BuildAnimations(pAScene, uScene, bonesTransforms);
    BuildLights(pAScene, uScene);
    return std::make_shared<SceneData>(uScene);;
}

void FBXLoader::BuildNodeMesh(aiScene const* pScene, aiNode const* pNode, sptr<SceneNode> outNode, uint32 parentIndex, SceneData& outScene)
{
    if (pNode->mNumMeshes > 1)
    {
        for (uint32 i = 0; i < pNode->mNumMeshes; ++i)
        {
            SceneNode newMesh = {};
            newMesh.MeshIndex = pNode->mMeshes[i];
            newMesh.type = SceneNodeType::MESH;
            newMesh.name = std::string(pNode->mName.C_Str()) + "_Child_" + std::to_string(i);
            newMesh.parent = parentIndex;
            sptr<SceneNode> newSptrNode = std::make_shared<SceneNode>(newMesh);
            outScene.allNode.push_back(newSptrNode);
            outNode->children.push_back(newSptrNode);
        }
    }
    else if (pNode->mNumMeshes == 1)
    {
        outNode->MeshIndex = pNode->mMeshes[0];
        outNode->type = SceneNodeType::MESH;
    }
}

sptr<SceneNode> FBXLoader::BuildNodesTree(aiScene const* pScene, aiNode const* pNode, int32 parentIndex, SceneData& outScene, Material& outMat, std::map<std::string, glm::mat4>& bonesTransform)
{
    SceneNode node;
    node.name = pNode->mName.C_Str();
    node.parent = parentIndex;
    node.transform = AIMatrixToGLMMatrix(pNode->mTransformation);
    uint32 selfIndex = outScene.allNode.size();
    sptr<SceneNode> sptrNode = std::make_shared<SceneNode>(node);
    if (std::string(pNode->mName.C_Str()).find("joint") == std::string::npos)
        outScene.allNode.push_back(sptrNode);
    else
        bonesTransform[node.name] = node.transform;
    if (pNode->mNumMeshes > 0)
        BuildNodeMesh(pScene, pNode, sptrNode, selfIndex, outScene);
    sptrNode->children.reserve(pNode->mNumChildren);
    for (uint32 i = 0; i < pNode->mNumChildren; i++)
    {
        sptrNode->children.push_back(BuildNodesTree(pScene, pNode->mChildren[i], selfIndex, outScene, outMat, bonesTransform));
    }
    if (parentIndex == -1)
        outScene.rootNode = sptrNode;
    return sptrNode;
}

void FBXLoader::BuildMeshs(aiScene const* pScene, SceneData& outScene, Material& outMat, std::map<std::string, glm::mat4>& bonesTransform)
{
    glm::mat4 parentMat = { 1.0f };
    std::vector<glm::mat4> meshWorldMatrices(pScene->mNumMeshes, glm::mat4(1.0f));
    std::vector<bool> meshMatrixAssigned(pScene->mNumMeshes, false);

    std::function<void(aiNode const*, glm::mat4 const&)> collectNodeTransforms;

    collectNodeTransforms = [&](aiNode const* pNode, glm::mat4 const& parentWorld)
        {
            if (!pNode) return;

            glm::mat4 const local = AIMatrixToGLMMatrix(pNode->mTransformation);
            glm::mat4 const world = parentWorld * local;

            for (uint32 i = 0; i < pNode->mNumMeshes; ++i)
            {
                uint32 const meshIdx = pNode->mMeshes[i];
                if (meshIdx < meshWorldMatrices.size() && !meshMatrixAssigned[meshIdx])
                {
                    meshWorldMatrices[meshIdx] = world;
                    meshMatrixAssigned[meshIdx] = true;
                }
            }

            for (uint32 c = 0; c < pNode->mNumChildren; ++c)
            {
                collectNodeTransforms(pNode->mChildren[c], world);
            }
        };

    collectNodeTransforms(pScene->mRootNode, glm::mat4(1.0f));

    glm::mat4 sceneFix(1.0f);
    if (pScene->mRootNode)
    {
        glm::mat4 const topConverter = ExtractTopLevelConverter(
            pScene->mRootNode,
            [](aiMatrix4x4 const& m) {
                return AIMatrixToGLMMatrix(m);
            });

        if (!IsNearlyIdentity(topConverter))
        {
            sceneFix = glm::inverse(topConverter);
        }
    }


    for (uint32 meshIndex = 0; meshIndex < pScene->mNumMeshes; ++meshIndex)
    {
        aiMesh* pMesh = pScene->mMeshes[meshIndex];
        if (!pMesh) continue;
        //aiMesh* pMesh = pScene->mMeshes[outScene.allNode[nodeIdx]->MeshIndex];

        std::vector<Ore::Vertex> vertices = {};
        std::vector<uint32> indices = {};

        vertices.reserve(pMesh->mNumVertices);
        indices.reserve(pMesh->mNumFaces * 3);

        BuildGeometry(pMesh, vertices, indices);

        std::vector<glm::mat4> bones = {};
        SceneMesh sMesh = {};
        //sMesh.meshMatrix = parentMat * outScene.allNode[meshIndex]->transform;
        //sMesh.meshMatrix =  outScene.allNode[nodeIdx]->transform;
        sMesh.meshMatrix = sceneFix * meshWorldMatrices[meshIndex];
        sMesh.vertices = vertices;
        sMesh.indices = indices;

        sMesh.name = "Mesh_" + outScene.allNode[meshIndex]->name;

        if (pMesh->HasBones())
        {
            BuildBones(bonesTransform, pMesh, sMesh);
        }

        if (outMat.textures.size() > 0 && pMesh->mMaterialIndex < outMat.textures.size())
        {
            for (std::map<Ore::TextureMaterialType, std::string>::iterator it = outMat.textures[pMesh->mMaterialIndex].begin(); it != outMat.textures[pMesh->mMaterialIndex].end(); ++it)
            {
                sptr<Ore::Texture> sharedTexture = AssetLoader::GetSharedTexture(it->second, it->first);
                if (sharedTexture)
                {
                    sMesh.meshTextures.push_back(sharedTexture);
                    sMesh.paths.push_back(it->second);
                }
            }
        }
        else
        {
            LoadDefaultsTextures(sMesh);
        }
        sMesh.ID = outScene.allMesh.size();
        outScene.allMesh.push_back(std::make_shared<SceneMesh>(sMesh));
    }
}

void FBXLoader::BuildGeometry(aiMesh const* pMesh, std::vector<Ore::Vertex>& vertices, std::vector<uint32>& indices)
{
    for (uint32 vIndex = 0; vIndex < pMesh->mNumVertices; ++vIndex)
    {
        Ore::Vertex v = {};
        v.position = { pMesh->mVertices[vIndex].x, pMesh->mVertices[vIndex].y, pMesh->mVertices[vIndex].z };
        if (pMesh->HasNormals())
            v.normal = { pMesh->mNormals[vIndex].x, pMesh->mNormals[vIndex].y, pMesh->mNormals[vIndex].z };
        if (pMesh->HasTextureCoords(0))
            v.texCoords = { pMesh->mTextureCoords[0][vIndex].x, pMesh->mTextureCoords[0][vIndex].y }; // Each vertex can have up to 8 uvs
        vertices.push_back(v);
    }
    for (uint32 fIndex = 0; fIndex < pMesh->mNumFaces; ++fIndex)
    {
        aiFace pFace = pMesh->mFaces[fIndex];
        for (uint32 iIndex = 0; iIndex < pFace.mNumIndices; ++iIndex)
            indices.push_back(pFace.mIndices[iIndex]);
    }

}

void FBXLoader::LoadEmbeddedTexture(std::string const& path, std::string& outPath, aiScene const* pScene, uint32& matIndex, Ore::TextureMaterialType type)
{
    aiTexture const* pText = pScene->GetEmbeddedTexture(path.c_str());
    if (pText == nullptr || pText->mHeight != 0)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Unable to Load Texture : " + path + " from FBX file : " + pScene->mName.C_Str());
        return;
    }
    std::string ext = pText->achFormatHint;
    if (ext.empty())
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Unable to Load Texture : " + path + " from FBX file : " + pScene->mName.C_Str() + " Extension Incorrect");
        return;
    }
    std::string textureName = "embedded_" + std::to_string(matIndex) + std::to_string(static_cast<uint32>(type)) + "." + ext;
    std::string fullPath = "res/textures/" + textureName;
    std::ofstream file(fullPath, std::ios::binary);
    file.write((const char*)pText->pcData, pText->mWidth);
    outPath = fullPath;
    file.close();
}


void FBXLoader::BuildMaterials(aiScene const* pScene, Material& outMat)
{
    outMat.textures.resize(pScene->mNumMaterials);
    for (uint32 i = 0; i < pScene->mNumMaterials; ++i)
    {
        aiMaterial* pMat = pScene->mMaterials[i];
        for (uint8 c = 0; c < static_cast<uint8>(Ore::TextureMaterialType::COUNT); ++c)
        {
            aiTextureType t = static_cast<aiTextureType>(FBXLoader::m_sTexTypes[c]);
            aiString texturePath;
            if (pMat->GetTextureCount(t) == 0 || pMat->GetTexture(t, 0, &texturePath) != AI_SUCCESS)
            {
                continue;
            }

            std::string path = texturePath.C_Str();
            if (texturePath.data[0] == '*')
            {
                std::string fullPath = "";
                LoadEmbeddedTexture(path, fullPath, pScene, i, static_cast<Ore::TextureMaterialType>(c));
                if (fullPath != "")
                {
                    outMat.textures[i][static_cast<Ore::TextureMaterialType>(c)] = fullPath;
                }
                continue;
            }
            if (texturePath.data[0] == 'r')
            {
                outMat.textures[i][static_cast<Ore::TextureMaterialType>(c)] = path;
                continue;
            }
            std::filesystem::path tempPath = path;
            std::string newPath = "res/textures/" + tempPath.filename().string();
            outMat.textures[i][static_cast<Ore::TextureMaterialType>(c)] = newPath;
        }
    }
}


void FBXLoader::BuildLights(aiScene const* pScene, SceneData& outScene)
{
    if (pScene->HasLights() == false)
        return;
    outScene.alllights.reserve(pScene->mNumLights);
    for (uint32 i = 0; i < pScene->mNumLights; ++i)
    {
        Ore::Light l = {};
        l.position = { pScene->mLights[i]->mPosition.x,pScene->mLights[i]->mPosition.y,pScene->mLights[i]->mPosition.z },
            l.constant = pScene->mLights[i]->mAttenuationConstant;
        l.quadratic = pScene->mLights[i]->mAttenuationQuadratic;
        l.linear = pScene->mLights[i]->mAttenuationLinear;
        l.color = Ore::Color(pScene->mLights[i]->mColorAmbient.r, pScene->mLights[i]->mColorAmbient.g, pScene->mLights[i]->mColorAmbient.b, 1.0f);
        outScene.alllights.push_back(std::make_shared<Ore::Light>(l));
    }
}

void FBXLoader::BuildBones(std::map<std::string, glm::mat4>& bonesTransform, aiMesh const* pMesh, SceneMesh& mesh)
{
    mesh.bonesOffest.reserve(pMesh->mNumBones);
    mesh.bonesTransform.reserve(pMesh->mNumBones);
    std::vector<glm::mat4> offsetMatrix = {};
    uint32 firstBoneIndex = 0;
    for (uint32 i = 0; i < pMesh->mNumBones; ++i)
    {
        aiBone* pBone = pMesh->mBones[i];
        mesh.bonesTransform.push_back(bonesTransform[pBone->mName.C_Str()]);
        mesh.bonesOffest.push_back(AIMatrixToGLMMatrix(pBone->mOffsetMatrix));
        for (uint32 wIndex = 0; wIndex < pBone->mNumWeights; ++wIndex)
        {
            int index = 0;
            int vID = pBone->mWeights[wIndex].mVertexId;
            for (uint32 n = 0; n < 4; ++n)
            {
                if (mesh.vertices[vID].boneIDS[n] != -1)
                    continue;

                index = n;
                break;
            }
            mesh.vertices[vID].weights[index] = static_cast<float>(pBone->mWeights[wIndex].mWeight);
            mesh.vertices[vID].boneIDS[index] = i;
        }
    }
}

void FBXLoader::BuildAnimations(aiScene const* pScene, SceneData& outScene, std::map<std::string, glm::mat4>& bonesTransform)
{
    if (pScene->HasAnimations() == false)
        return;
    outScene.isAnimated = true;
    outScene.animations.reserve(pScene->mNumAnimations);
    for (uint32 i = 0; i < pScene->mNumAnimations; ++i)
    {
        Animation anim = {};
        aiAnimation* pAnim = pScene->mAnimations[i];
        anim.duration = pAnim->mDuration;
        anim.name = pAnim->mName.C_Str();
        uint8 ctrlIndex = 0;
        uint8 bonesIndex = 0;
        for (uint32 animCount = 0; animCount < pAnim->mNumChannels; ++animCount)
        {
            BuildAnimationsChannels(bonesTransform, pAnim, anim, animCount, ctrlIndex, bonesIndex);
        }
        outScene.animations.push_back(std::make_shared<Animation>(anim));
    }
}

void FBXLoader::BuildAnimationsChannels(std::map<std::string, glm::mat4>& bonesTransform, aiAnimation const* pAnim, Animation& outAnim, uint32 channelID, uint8 ctrID, uint8 boneID)
{
    AnimationChannel channel = {};
    aiNodeAnim* pAnimNode = pAnim->mChannels[channelID];
    if (bonesTransform.find(pAnimNode->mNodeName.C_Str()) == bonesTransform.end())
        return;

    channel.sceneNodeImpacted = boneID++;
    channel.frameCount = pAnimNode->mNumPositionKeys;
    channel.positionKeys.reserve(pAnimNode->mNumPositionKeys);
    channel.rotationKeys.reserve(pAnimNode->mNumRotationKeys);
    channel.scalingKeys.reserve(pAnimNode->mNumScalingKeys);
    for (uint32 k = 0; k < pAnimNode->mNumPositionKeys; ++k)
    {
        AnimationChannel::FrameVec3 f = {};
        f.time = pAnimNode->mPositionKeys[k].mTime;
        f.vec = { pAnimNode->mPositionKeys[k].mValue.x, pAnimNode->mPositionKeys[k].mValue.y,pAnimNode->mPositionKeys[k].mValue.z };
        channel.positionKeys.push_back(f);
    }
    for (uint32 k = 0; k < pAnimNode->mNumRotationKeys; ++k)
    {
        AnimationChannel::FrameQuat f = {};
        f.time = pAnimNode->mRotationKeys[k].mTime;
        f.quaternion = { pAnimNode->mRotationKeys[k].mValue.x, pAnimNode->mRotationKeys[k].mValue.y,pAnimNode->mRotationKeys[k].mValue.z,pAnimNode->mRotationKeys[k].mValue.w };
        channel.rotationKeys.push_back(f);
    }
    for (uint32 k = 0; k < pAnimNode->mNumScalingKeys; ++k)
    {
        AnimationChannel::FrameVec3 f = {};
        f.time = pAnimNode->mScalingKeys[k].mTime;
        f.vec = { pAnimNode->mScalingKeys[k].mValue.x, pAnimNode->mScalingKeys[k].mValue.y,pAnimNode->mScalingKeys[k].mValue.z };
        channel.scalingKeys.push_back(f);
    }
    outAnim.animationTransform.push_back(std::make_shared<AnimationChannel>(channel));
}
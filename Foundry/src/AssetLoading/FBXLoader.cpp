#include "AssetLoading/FBXLoader.h"
#include "Servers/GraphicServer.h"

#include <fstream>
#include <Mesh.h>
#include <Logger.hpp>
#include <MathUtils.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

void FBXLoader::LoadTextures(FBXLoader::Material& materials,std::vector<sptr<Ore::Texture>>& vect, uint32 matIndex)
{
    for (std::map<Ore::TextureMaterialType, std::string>::iterator it = materials.textures[matIndex].begin(); it != materials.textures[matIndex].end(); ++it)
    {
        sptr<Ore::Texture> text = std::make_shared<Ore::Texture>(Ore::Texture(it->second, Ore::TextureType::TYPE_2D, it->first));
        vect.push_back(text);
    }
}

void FBXLoader::LoadDefaultsTextures(std::vector<sptr<Ore::Texture>>& vect)
{
    sptr<Ore::Texture> text = std::make_shared<Ore::Texture>("res/textures/NormalMap.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::NORMAL);
    vect.push_back(text);
    sptr<Ore::Texture> text2 = std::make_shared<Ore::Texture>("res/textures/diffuse.jpg", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE);
    vect.push_back(text2);
    sptr<Ore::Texture> text3 = std::make_shared<Ore::Texture>("res/textures/specular.jpg", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::SPECULAR);
    vect.push_back(text3);
}

sptr<SceneData> FBXLoader::LoadFile(std::string const& path)
{
    Assimp::Importer importer = Assimp::Importer();
    int importFlags = aiProcess_CalcTangentSpace |
                         aiProcess_Triangulate |
                         aiProcess_JoinIdenticalVertices |
                         aiProcess_SortByPType;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    aiScene const* pAScene = importer.ReadFile(path.c_str(), importFlags);

    if (pAScene == nullptr)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Failed Loading " + path + " file");
        return nullptr;
    }    
    SceneData uScene = {};
    Material allTextures = {};
    uint32 nodeCount = 0;
    BuildMaterials(pAScene, allTextures);
    BuildNodesTree(pAScene, pAScene->mRootNode, -1, uScene,allTextures);
    BuildMeshs(pAScene,uScene,allTextures);

    BuildAnimations(pAScene, uScene);
    BuildLights(pAScene, uScene);
    return std::make_shared<SceneData>(uScene);;
}


sptr<SceneNode> FBXLoader::BuildNodesTree(aiScene const* pScene, aiNode const* pNode, int32 parentIndex, SceneData& outScene, Material& outMat)
{
    SceneNode node;
    node.name = pNode->mName.C_Str();
    node.parent = parentIndex;
    node.transform = AIMatrixToGLMMatrix(pNode->mTransformation);
    uint32 selfIndex = outScene.allNode.size();
    sptr<SceneNode> sptrNode = std::make_shared<SceneNode>(node);
    outScene.allNode.push_back(sptrNode);
    if (pNode->mNumMeshes > 1)
    {
        for (uint32 i = 0; i < pNode->mNumMeshes; ++i)
        {
            SceneNode newMesh = {};
            newMesh.MeshIndex = pNode->mMeshes[i];
            newMesh.type = SceneNodeType::MESH;
            newMesh.parent = selfIndex;
            sptr<SceneNode> newSptrNode = std::make_shared<SceneNode>(newMesh);
            outScene.allNode.push_back(newSptrNode);
        }
    }else if (pNode->mNumMeshes == 1)
    {
        sptrNode->MeshIndex = pNode->mMeshes[0];
        sptrNode->type = SceneNodeType::MESH;
    }
    sptrNode->children.reserve(pNode->mNumChildren);
    for (uint32 i = 0; i < pNode->mNumChildren;i++)
    {
        sptrNode->children.push_back(BuildNodesTree(pScene,pNode->mChildren[i], selfIndex,outScene,outMat));
        
    }
    return sptrNode;
}

void FBXLoader::BuildMeshs(aiScene const* pScene, SceneData& outScene, Material& outMat)
{
    // Load Vertices
    for (uint32 nodeIdx = 0; nodeIdx < outScene.allNode.size(); ++nodeIdx)
    {
        if (outScene.allNode[nodeIdx]->MeshIndex == -1)
            continue;
        aiMesh* pMesh = pScene->mMeshes[outScene.allNode[nodeIdx]->MeshIndex];
        std::vector<Ore::Vertex> vertices = {};
        std::vector<uint32> indices = {};
        vertices.reserve(pMesh->mNumVertices);
        indices.reserve(pMesh->mNumFaces * 3);
        BuildGeometry(pMesh,vertices, indices);
        std::vector<glm::mat4> bones = {};
        SceneMesh sMesh = {};
        sMesh.meshMatrix = outScene.allNode[nodeIdx]->transform;
        sMesh.vertices = vertices;
        sMesh.indices = indices;
        if (pMesh->HasBones())
        {
            BuildBones(outScene,pMesh,sMesh);
        }
        std::vector<sptr<Ore::Texture>> textures = {};
        if (outMat.textures.size() > 0 && pMesh->mMaterialIndex < outMat.textures.size())
        {
            LoadTextures(outMat, textures, pMesh->mMaterialIndex);
        }
        else
            LoadDefaultsTextures(textures);
        sMesh.meshTextures = textures;

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
        {
            indices.push_back(pFace.mIndices[iIndex]);
        }
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
    outMat.textures.reserve(pScene->mNumMaterials);
    for (uint32 i = 0; i < pScene->mNumMaterials; ++i)
    {
        aiMaterial* pMat = pScene->mMaterials[i];
        for (uint8 c = 0; c < static_cast<uint8>(Ore::TextureMaterialType::COUNT); ++c)
        {
            aiTextureType t = static_cast<aiTextureType>(FBXLoader::m_sTexTypes[c]);
            aiString texturePath;
            if (pMat->GetTextureCount(t) == 0 || pMat->GetTexture(t, 0, &texturePath) != AI_SUCCESS)
            {
                Logger::LogWithLevel(LogLevel::WARNING, std::string("No Textures In FBX FIle") + pScene->mName.C_Str());
                continue;
            }

            std::string path = texturePath.C_Str();
            if (texturePath.data[0] != '*')
            {
                outMat.textures[i][static_cast<Ore::TextureMaterialType>(c)] = path;
                continue;
            }
            std::string fullPath = "";
            LoadEmbeddedTexture(path, fullPath, pScene, i, static_cast<Ore::TextureMaterialType>(c));
            if (fullPath != "")
                outMat.textures[i][static_cast<Ore::TextureMaterialType>(c)] = fullPath;
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

void FBXLoader::BuildBones(SceneData& outScene, aiMesh const* pMesh, SceneMesh& mesh)
{
    mesh.bonesOffest.reserve(pMesh->mNumBones);
    mesh.bonesTransform.reserve(pMesh->mNumBones);
    std::vector<glm::mat4> offsetMatrix = {};
    uint32 firstBoneIndex = 0;
    for (uint32 i = 0; i < pMesh->mNumBones; ++i)
    {
        aiBone* pBone = pMesh->mBones[i];
        for (uint32 k = 0; k < outScene.allNode.size(); ++k)
        {
            if (outScene.allNode[k]->name == std::string(pBone->mName.C_Str()))
            {
                if (i == 0) firstBoneIndex = k;
                outScene.allNode[k]->type = BONE;
                outScene.allNode[k]->boneIndexInMesh = i;
                mesh.bonesTransform.push_back(outScene.allNode[k]->transform);
                break;
            }
        }
        mesh.bonesOffest.push_back(AIMatrixToGLMMatrix(pBone->mOffsetMatrix));
        for (uint32 wIndex = 0; wIndex < pBone->mNumWeights; ++wIndex)
        {
            int index = 0;
            int vID = pBone->mWeights[wIndex].mVertexId;
            for (uint32 n = 0; n < 4; ++n)
            {
                if (mesh.vertices[vID].boneIDS[n] == -1)
                {
                    index = n;
                    break;
                }
            }
            mesh.vertices[vID].weights[index] = static_cast<float>(pBone->mWeights[wIndex].mWeight);
            mesh.vertices[vID].boneIDS[index] = i;
        }
    }
}

void FBXLoader::BuildAnimations(aiScene const* pScene, SceneData& outScene)
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
            BuildAnimationsChannels(outScene,pAnim, anim, animCount, ctrlIndex,bonesIndex);
        }
        outScene.animations.push_back(std::make_shared<Animation>(anim));
    }
}

void FBXLoader::BuildAnimationsChannels(SceneData& outScene,aiAnimation const* pAnim, Animation& outAnim, uint32 channelID, uint8 ctrID, uint8 boneID)
{
    AnimationChannel channel = {};
    aiNodeAnim* pAnimNode = pAnim->mChannels[channelID];
    for (uint32 i = 0; i < outScene.allNode.size(); ++i)
    {
        if (std::string(pAnimNode->mNodeName.C_Str()) == outScene.allNode[i]->name)
        {
            if (outScene.allNode[i]->type != BONE)
                return;
            channel.sceneNodeImpacted = boneID++;
            channel.isController = false;
            break;
        }
    }
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
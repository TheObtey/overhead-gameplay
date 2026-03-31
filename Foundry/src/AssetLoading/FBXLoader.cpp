#include "AssetLoading/FBXLoader.h"

#include <fstream>
#include <Mesh.h>
#include <Logger.hpp>

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

void FBXLoader::LoadTextures(FBXLoader::Material& materials,std::vector<sptr<Texture>>& vect, std::vector<Texture*>& tempVect, uint32 matIndex)
{
    for (std::map<TextureMaterialType, std::string>::iterator it = materials.textures[matIndex].begin(); it != materials.textures[matIndex].end(); ++it)
    {
        sptr<Texture> text = std::make_shared<Texture>(Texture(it->second, TextureType::TYPE_2D, it->first));
        vect.push_back(text);
        tempVect.push_back(text.get());
    }
}

void FBXLoader::LoadDefaultsTextures(std::vector<sptr<Texture>>& vect, std::vector<Texture*>& tempVect)
{
    sptr<Texture> text = std::make_shared<Texture>("res/textures/NormalMap.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL);
    vect.push_back(text);
    tempVect.push_back(text.get());
    sptr<Texture> text2 = std::make_shared<Texture>("res/textures/diffuse.jpg", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);
    vect.push_back(text2);
    tempVect.push_back(text2.get());
    sptr<Texture> text3 = std::make_shared<Texture>("res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);
    vect.push_back(text3);
    tempVect.push_back(text3.get());
}

sptr<SceneData> FBXLoader::LoadFile(std::string const& path)
{
    if (m_loadedFiles.contains(path))
        return m_loadedFiles[path];
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
    BuildLights(pAScene, uScene);
    BuildAnimations(pAScene, uScene);
    BuildNodesTree(pAScene, pAScene->mRootNode, -1, uScene,allTextures);
    BuildMeshs(pAScene,uScene,allTextures);
    m_loadedFiles[path] = std::make_shared<SceneData>(uScene);
    return m_loadedFiles[path];
}


uint32 FBXLoader::BuildNodesTree(aiScene const* pScene, aiNode const* pNode, int32 parentIndex, SceneData& outData, Material& outMat)
{
    SceneNode node;
    node.name = pNode->mName.C_Str();
    node.parent = parentIndex;
    node.transform = AIMatrixToGLMMatrix(pNode->mTransformation);
    uint32 selfIndex = outData.allNode.size();
    node.meshesIndex.reserve(pNode->mNumMeshes);
    if (pNode->mNumMeshes > 0) node.type = SceneNodeType::MESH;
    for (uint32 i = 0; i < pNode->mNumMeshes; ++i)
    {
        node.meshesIndex.push_back(pNode->mMeshes[i]);
    }
    sptr<SceneNode> sptrNode = std::make_shared<SceneNode>(node);
    outData.allNode.push_back(sptrNode);
    if (parentIndex == -1) outData.rootNode = sptrNode;
    for (uint32 i = 0; i < pNode->mNumChildren;i++)
    {
        uint32 cIndex = BuildNodesTree(pScene,pNode->mChildren[i], selfIndex,outData,outMat);
        sptrNode->children.push_back(cIndex);
    }
    return selfIndex;
}

void FBXLoader::BuildMeshs(aiScene const* pScene, SceneData& outData, Material& outMat)
{
    // Load Vertices
    for (uint32 nodeIdx = 0; nodeIdx < outData.allNode.size(); ++nodeIdx)
    {
        for (uint32 i = 0; i < outData.allNode[nodeIdx]->meshesIndex.size(); ++i)
        {
            aiMesh* pMesh = pScene->mMeshes[outData.allNode[nodeIdx]->meshesIndex[i]];
            std::vector<Vertex> vertices = {};
            std::vector<uint32> indices = {};
            vertices.reserve(pMesh->mNumVertices);
            indices.reserve(pMesh->mNumFaces * 3);
            BuildGeometry(pMesh,vertices, indices);
            Geometry geo = Geometry(vertices, indices);
            if (pMesh->HasBones())
            {
                std::vector<glm::mat4> bones = {};
                BuildBones(outData,pMesh,vertices,indices,bones);
            }
            std::vector<Texture*> meshCreation = {};
            std::vector<sptr<Texture>> textures = {};
            if (outMat.textures.size() > 0 && pMesh->mMaterialIndex < outMat.textures.size())
            {
                LoadTextures(outMat, textures, meshCreation, pMesh->mMaterialIndex);
            }
            else
                LoadDefaultsTextures(textures, meshCreation);
            outData.allMesh.push_back(std::make_shared<Mesh>(geo,meshCreation, outData.allNode[nodeIdx]->transform));
            outData.allTextures.insert(outData.allTextures.end(), textures.begin(), textures.end());
            meshCreation.clear();
        }
    }
}

void FBXLoader::BuildGeometry(aiMesh const* pMesh, std::vector<Vertex>& vertices, std::vector<uint32>& indices)
{
    for (uint32 vIndex = 0; vIndex < pMesh->mNumVertices; ++vIndex)
    {
        Vertex v = {};
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

void FBXLoader::LoadEmbeddedTexture(std::string const& path, std::string& outPath, aiScene const* pScene, uint32& matIndex, TextureMaterialType type)
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
        for (uint8 c = 0; c < static_cast<uint8>(TextureMaterialType::COUNT); ++c)
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
                outMat.textures[i][static_cast<TextureMaterialType>(c)] = path;
                continue;
            }
            std::string fullPath = "";
            LoadEmbeddedTexture(path, fullPath, pScene, i, static_cast<TextureMaterialType>(c));
            if (fullPath != "")
                outMat.textures[i][static_cast<TextureMaterialType>(c)] = fullPath;
        }
    }
}


void FBXLoader::BuildLights(aiScene const* pScene, SceneData& outData)
{
    if (pScene->HasLights() == false)
        return;
    outData.alllights.reserve(pScene->mNumLights);
    for (uint32 i = 0; i < pScene->mNumLights; ++i)
    {
        Light l = {};
        l.position = { pScene->mLights[i]->mPosition.x,pScene->mLights[i]->mPosition.y,pScene->mLights[i]->mPosition.z },
        l.constant = pScene->mLights[i]->mAttenuationConstant;
        l.quadratic = pScene->mLights[i]->mAttenuationQuadratic;
        l.linear = pScene->mLights[i]->mAttenuationLinear;
        l.color = Color(pScene->mLights[i]->mColorAmbient.r, pScene->mLights[i]->mColorAmbient.g, pScene->mLights[i]->mColorAmbient.b, 1.0f);
        outData.alllights.push_back(l);
    }
}

void FBXLoader::BuildBones(SceneData& outData, aiMesh const* pMesh, std::vector<Vertex>& vertices, std::vector<uint32>& indices, std::vector<glm::mat4>& bones)
{
    bones.reserve(pMesh->mNumBones);
    for (uint32 i = 0; i < pMesh->mNumBones; ++i)
    {
        aiBone* pBone = pMesh->mBones[i];
        for (uint32 k = 0; k < outData.allNode.size(); ++k)
        {
            if (outData.allNode[k]->name == std::string(pBone->mName.C_Str()))
            {
                outData.allNode[k]->type = BONE;
                outData.allNode[k]->boneIndexInMesh = i;
            }
        }
        bones.push_back(AIMatrixToGLMMatrix(pBone->mOffsetMatrix));
        for (uint32 wIndex = 0; wIndex < pBone->mNumWeights; ++wIndex)
        {
            int index = 0;
            int vID = pBone->mWeights[wIndex].mVertexId;
            for (uint32 n = 0; n < 4; ++n)
            {
                if (vertices[vID].boneIDS[n] == -1)
                    index = n;
            }
            vertices[vID].weights[index] = static_cast<float>(pBone->mWeights[wIndex].mWeight);
            vertices[vID].boneIDS[index] = i;
        }
    }
}


void FBXLoader::BuildAnimations(aiScene const* pScene, SceneData& outData)
{
    if (pScene->HasAnimations() == false)
        return;
    outData.isAnimated = true;
    outData.animations.reserve(pScene->mNumAnimations);
    for (uint32 i = 0; i < pScene->mNumAnimations; ++i)
    {
        Animation anim = {};
        aiAnimation* pAnim = pScene->mAnimations[i];
        anim.duration = pAnim->mDuration;
        anim.name = pAnim->mName.C_Str();

        for (uint32 animCount = 0; animCount < pAnim->mNumChannels; animCount++)
        {
            BuildAnimationsChannles(pAnim, anim, animCount);
        }
        outData.animations.push_back(anim);
    }
}

void FBXLoader::BuildAnimationsChannles(aiAnimation const* pAnim, Animation& outAnim, uint32 channelID)
{
    AnimationChannel channel = {};
    aiNodeAnim* pAnimNode = pAnim->mChannels[channelID];
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
    outAnim.animationTransform.push_back(channel);
}
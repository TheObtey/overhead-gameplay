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
        aiTextureType_EMISSIVE
};

glm::mat4x4 AIMatrixToGLMMatrix(aiMatrix4x4 const& matrix)
{
    glm::mat4x4 out = {};
    memcpy(&out, &matrix, sizeof(aiMatrix4x4));
    return glm::transpose(out);
}

void FBXLoader::LoadTextures(FBXSceneData& outData, std::vector<sptr<Texture>>& vect, std::vector<Texture*>& tempVect, uint32 matIndex)
{
    FBXLoader::Material meshMat = {};
    if (outData.textures.size() != 0)
    {
        meshMat = outData.textures[matIndex];
        for (std::map<TextureMaterialType, std::string>::iterator it = meshMat.textures.begin(); it != meshMat.textures.end(); ++it)
        {
            sptr<Texture> text = std::make_shared<Texture>(Texture(it->second, TextureType::TYPE_2D, it->first));
            vect.push_back(text);
            tempVect.push_back(text.get());
        }
    }
    if (meshMat.textures.size() == 0 || outData.textures.size() == 0)
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
}

sptr<SceneData> FBXLoader::ConvertInGlobalSceneData(FBXSceneData& outData)
{
    SceneData outScene = {};
    outScene.animations = outData.animations;
    outScene.lights = outData.lights;
    outScene.meshes.reserve(outData.meshs.size());
    for (uint32 i = 0; i < outData.nodes.size(); ++i)
    {
        for (uint32 meshCount = 0; meshCount < outData.nodes[i]->meshesIndex.size(); ++meshCount)
        {
            FBXLoader::FBXMesh mesh = *outData.meshs[outData.nodes[i]->meshesIndex[meshCount]];
            std::vector<sptr<Texture>> texts = {};
            std::vector<Texture*> meshcreationTexts = {};
            LoadTextures(outData, texts, meshcreationTexts, mesh.matIndex);
            Geometry geo = Geometry(mesh.vertices, mesh.indices);
            glm::mat4x4 mat = outData.nodes[i]->transform;
            SceneMeshs outMesh = {};
            outMesh.mesh = std::make_shared<Mesh>(geo, meshcreationTexts, mat);
            outMesh.mesh->SetBones(mesh.bones);
            outMesh.textureOfMeshes = texts;
            outScene.meshes.push_back(outMesh);
            meshcreationTexts.clear();
        }
    }
    return std::make_shared<SceneData>(outScene);
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

    aiScene const* pAScene = importer.ReadFile(path.c_str(), importFlags);

    if (pAScene == nullptr)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Failed Loading " + path + " file");
        return nullptr;
    }
    
    FBXLoader::FBXSceneData uScene = {};

    BuildNodes(pAScene, pAScene->mRootNode, -1, uScene);
    BuildMeshs(pAScene, uScene);
    BuildMaterials(pAScene, uScene);
    BuildLights(pAScene, uScene);
    BuildAnimations(pAScene, uScene);

    sptr<SceneData> outScene = ConvertInGlobalSceneData(uScene);

    m_loadedFiles[path] = outScene;

    return outScene;
}


uint32 FBXLoader::BuildNodes(aiScene const* pScene, aiNode const* pNode, int32 parentIndex, FBXSceneData& outData)
{
    Node node;
    node.name = pNode->mName.C_Str();
    node.parent = parentIndex;
    node.transform = AIMatrixToGLMMatrix(pNode->mTransformation);
    uint32 selfIndex = outData.nodes.size();
    node.meshesIndex.reserve(pNode->mNumMeshes);
    for (uint32 i = 0; i < pNode->mNumMeshes; ++i)
    {
        node.meshesIndex.push_back(pNode->mMeshes[i]);
    }

    outData.nodes.push_back(std::make_shared<Node>(node));
    for (uint32 i = 0; i < pNode->mNumChildren;i++)
    {
        uint32 cIndex = BuildNodes(pScene,pNode->mChildren[i], selfIndex,outData);
        node.children.push_back(cIndex);
    }
    return selfIndex;
}

void FBXLoader::BuildMeshs(aiScene const* pScene, FBXSceneData& outData)
{
    for (uint32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        // Load Vertices
        aiMesh* pMesh = pScene->mMeshes[i];
        FBXLoader::FBXMesh mesh = {};
        mesh.vertices.reserve(pMesh->mNumVertices);
        mesh.indices.reserve(pMesh->mNumFaces * 3);

        mesh.matIndex = static_cast<int32>(pMesh->mMaterialIndex);
        for (uint32 vIndex = 0; vIndex < pMesh->mNumVertices; ++vIndex)
        {
            Vertex v = {};
            v.position = { pMesh->mVertices[vIndex].x, pMesh->mVertices[vIndex].y, pMesh->mVertices[vIndex].z };

            if (pMesh->HasNormals())
                v.normal = { pMesh->mNormals[vIndex].x, pMesh->mNormals[vIndex].y, pMesh->mNormals[vIndex].z };
            if (pMesh->HasTextureCoords(0))
                v.texCoords = { pMesh->mTextureCoords[0][vIndex].x, pMesh->mTextureCoords[0][vIndex].y }; // Each vertex can have up to 8 uvs
            mesh.vertices.push_back(v);
        }


        for (uint32 fIndex = 0; fIndex < pMesh->mNumFaces; ++fIndex)
        {
            aiFace pFace = pMesh->mFaces[fIndex];
            for (uint32 iIndex = 0; iIndex < pFace.mNumIndices; ++iIndex)
            {
                mesh.indices.push_back(pFace.mIndices[iIndex]);
            }

        }

        if (pMesh->HasBones())
            BuildBones(pMesh, mesh);

        outData.meshs.push_back(std::make_shared<FBXMesh>(mesh));
    }
}

void FBXLoader::BuildMaterials(aiScene const* pScene, FBXSceneData& outData)
{

    for (uint32 i = 0; i < pScene->mNumMaterials; ++i)
    {
        aiMaterial* pMat = pScene->mMaterials[i];
        Material mat = {};
        for (uint8 c = 0; c < 4; ++c)
        {
            aiTextureType t = static_cast<aiTextureType>(FBXLoader::m_sTexTypes[c]);
            if (pMat->GetTextureCount(t) == 0)
                continue;

            aiString texturePath;
            if (pMat->GetTexture(t, 0, &texturePath) != AI_SUCCESS)
                continue;
            
            std::string path = texturePath.C_Str();

            if (texturePath.data[0] != '*')
            {
                mat.textures[static_cast<TextureMaterialType>(c)] = path;
                continue;
            }

            aiTexture const* pText = pScene->GetEmbeddedTexture(texturePath.C_Str());
            if (pText == nullptr)
                continue;

            if (pText->mHeight != 0)
            {
                Logger::LogWithLevel(LogLevel::WARNING, "Unable to Load Texture : " + path + " from FBX file : " + pScene->mName.C_Str());
                continue;
            }
            std::string ext = pText->achFormatHint;
            if (ext.empty())
            {
                Logger::LogWithLevel(LogLevel::WARNING, "Unable to Load Texture : " + path + " from FBX file : " + pScene->mName.C_Str() + " Extension Incorrect");
                continue;
            }

            std::string textureName = "embedded_" + std::to_string(i) + std::to_string(c) + "." + ext;
            std::string fullPath = "res/textures/" + textureName;
            std::ofstream file(fullPath, std::ios::binary);
            file.write((const char*)pText->pcData, pText->mWidth);

            mat.textures[static_cast<TextureMaterialType>(c)] = fullPath;
        }

        outData.textures.push_back(mat);
    }
}


void FBXLoader::BuildLights(aiScene const* pScene, FBXSceneData& outData)
{
    if (pScene->HasLights() == false)
        return;

    outData.lights.reserve(pScene->mNumLights);
    for (uint32 i = 0; i < pScene->mNumLights; ++i)
    {
        Light l = {};
        l.position = { pScene->mLights[i]->mPosition.x,pScene->mLights[i]->mPosition.y,pScene->mLights[i]->mPosition.z },
        l.constant = pScene->mLights[i]->mAttenuationConstant;
        l.quadratic = pScene->mLights[i]->mAttenuationQuadratic;
        l.linear = pScene->mLights[i]->mAttenuationLinear;
        l.color = Color(pScene->mLights[i]->mColorAmbient.r, pScene->mLights[i]->mColorAmbient.g, pScene->mLights[i]->mColorAmbient.b, 1.0f);
        outData.lights.push_back(l);
    }
}

void FBXLoader::BuildBones(aiMesh const* pMesh, FBXMesh& outMesh)
{
    outMesh.bones.reserve(pMesh->mNumBones);
    for (uint32 i = 0; i < pMesh->mNumBones; ++i)
    {
        aiBone* pBone = pMesh->mBones[i];
        outMesh.bones.push_back(AIMatrixToGLMMatrix(pBone->mOffsetMatrix));
        for (uint32 wIndex = 0; wIndex < pBone->mNumWeights; ++wIndex)
        {
            int index = 0;
            for (uint32 n = 0; n < 4; ++n)
            {
                if (outMesh.vertices[pBone->mWeights[wIndex].mVertexId].boneIDS[n] == -1)
                    index = n;
            }
            outMesh.vertices[pBone->mWeights[wIndex].mVertexId].weights[index] = pBone->mWeights[wIndex].mWeight;
            outMesh.vertices[pBone->mWeights[wIndex].mVertexId].boneIDS[index] = i;
        }
    }
}


void FBXLoader::BuildAnimations(aiScene const* pScene, FBXSceneData& outData)
{
    if (pScene->HasAnimations() == false)
        return;

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
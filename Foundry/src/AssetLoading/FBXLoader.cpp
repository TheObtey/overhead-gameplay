#include "AssetLoading/FBXLoader.h"

#include <fstream>
#include <Mesh.h>

#include <Logger.hpp>


glm::mat4x4 AIMatrixToGLMMatrix(aiMatrix4x4 const& matrix)
{
    glm::mat4x4 out = {};
    memcpy(&out, &matrix, sizeof(aiMatrix4x4));
    return glm::transpose(out);
}


uptr<FBXLoader::SceneData> FBXLoader::LoadFile(std::string const& path)
{
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
    
    FBXLoader::SceneData uScene = {};

    BuildNodes(pAScene->mRootNode, -1, uScene);
    BuildMeshs(pAScene, uScene);
    BuildMaterials(pAScene, uScene);

    m_loadedFiles[path] = uScene;

    return std::make_unique<FBXLoader::SceneData>(uScene);
}


uint32 FBXLoader::BuildNodes(aiNode const* pNode, int32 parentIndex, FBXLoader::SceneData& outData)
{
    Node node;
    node.parent = parentIndex;
    node.transform = AIMatrixToGLMMatrix(pNode->mTransformation);
    uint32 selfIndex = outData.nodes.size();
    outData.nodes.push_back(node);
    
    for (uint32 i = 0; i < pNode->mNumChildren;i++)
    {
        uint32 cIndex = BuildNodes(pNode->mChildren[i], selfIndex,outData);
        node.children.push_back(cIndex);
    }
    return selfIndex;
}

void FBXLoader::BuildMeshs(aiScene const* pScene, FBXLoader::SceneData& outData)
{
    for (uint32 i = 0; i < pScene->mNumMeshes; ++i)
    {
        // Load Vertices
        aiMesh* pMesh = pScene->mMeshes[i];
        FBXLoader::Mesh mesh = {};
        mesh.matIndex = static_cast<int32>(pMesh->mMaterialIndex);
        for (uint32 vIndex = 0; vIndex < pMesh->mNumVertices; ++vIndex)
        {
            Vertex v = {};
            v.position = { pMesh->mVertices[vIndex].x, pMesh->mVertices[vIndex].y, pMesh->mVertices[vIndex].z };

            if (pMesh->HasTextureCoords(0))
                v.normal = { pMesh->mNormals[vIndex].x, pMesh->mNormals[vIndex].y, pMesh->mNormals[vIndex].z };
            if (pMesh->HasNormals())
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
        outData.meshs.push_back(mesh);
    }
}

void FBXLoader::BuildMaterials(aiScene const* pScene, SceneData& outData)
{
    for (uint32 i = 0; i < pScene->mNumMaterials; ++i)
    {
        // Load Vertices
        aiMaterial* pMat = pScene->mMaterials[i];
        Material mat = {};
        aiTextureType texTypes[] = {
            aiTextureType_DIFFUSE,
            aiTextureType_SPECULAR,
            aiTextureType_NORMALS,
            aiTextureType_HEIGHT,
            aiTextureType_EMISSIVE
        };

        for (uint8 c = 0; c < 4; ++c)
        {
            if (pMat->GetTextureCount(texTypes[c]) == 0)
                continue;

            aiString texturePath;
            if ( pMat->GetTexture(texTypes[c],0,&texturePath) == AI_SUCCESS )
            {
                std::string path = texturePath.C_Str();
                

                if ('*' == texturePath.data[0])
                {
                    aiTexture const* pText = pScene->GetEmbeddedTexture(texturePath.C_Str());
                    if (pText == nullptr)
                    {   
                        continue;
                    }
                    
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

                    // Create Texture File
                    std::ofstream file(fullPath, std::ios::binary);
                    file.write((const char*)pText->pcData, pText->mWidth);

                    mat.textures[static_cast<TextureMaterialType>(c)] = path;

                }
                else
                {
                    mat.textures[static_cast<TextureMaterialType>(c)] = path;
                }
            }

        }
    }
}
#include "AssetLoading/FBXLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postProcess.h>

#include <Logger.hpp>

glm::mat4x4 AIMatrixToGLMMatrix(aiMatrix4x4 const& matrix)
{
    glm::mat4x4 out = {};
    memcpy(&out, &matrix, sizeof(aiMatrix4x4));
    return glm::transpose(out);
}


uptr<SceneData> FBXLoader::LoadFile(std::string const& path)
{
	Assimp::Importer importer;
    uint32 importFlags = aiProcess_CalcTangentSpace |
                         aiProcess_Triangulate |
                         aiProcess_JoinIdenticalVertices |
                         aiProcess_SortByPType;

    aiScene const* pAScene = importer.ReadFile(path, importFlags);

    if (pAScene == nullptr)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Failed Loading " + path + " file");
    }
    
    uptr<SceneData> uScene = std::make_unique<SceneData>();

    BuildNodes(pAScene->mRootNode, -1, *uScene);

    return std::move(uScene);

}
uint32 FBXLoader::BuildNodes(aiNode const* pNode, int32 parentIndex, SceneData& outData)
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
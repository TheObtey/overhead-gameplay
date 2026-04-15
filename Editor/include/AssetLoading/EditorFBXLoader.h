#ifndef EDITOR_ASSET_LOADING_EDITOR_FBX_LOADER_H__
#define EDITOR_ASSET_LOADING_EDITOR_FBX_LOADER_H__

#include "AssetLoading/EditorAssetLoader.h"

#include <assimp/matrix4x4.h>

struct aiScene;
struct aiNode;
struct aiMesh;

class EditorFBXLoader
{
public:
	static sptr<EditorSceneData> LoadFile(std::string const& path);

private:
	static void BuildMeshes(aiScene const* pScene, std::string const& sourcePath, EditorSceneData& outScene);
	static glm::mat4 AIMatrixToGLMMatrix(aiMatrix4x4 const& matrix);
	static std::string ResolveTexturePath(std::string const& sourcePath, std::string const& texturePath);
	static std::string ExtractEmbeddedTexture(aiScene const* pScene, std::string const& texturePath, uint32 meshIndex, uint32 type);
};

#endif // EDITOR_ASSET_LOADING_EDITOR_FBX_LOADER_H__
#ifndef EDITOR_ASSET_LOADING_EDITOR_ASSET_LOADER_H__
#define EDITOR_ASSET_LOADING_EDITOR_ASSET_LOADER_H__

#include <Define.h>
#include <GeometryFactory.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

struct EditorTextureRef
{
	uint32 textureType = 0;
	std::string path;
};

struct EditorSceneMeshData
{
	GeoInfo geometry;
	glm::mat4 meshMatrix = glm::mat4(1.0f);
	std::vector<EditorTextureRef> textures;
};

struct EditorSceneData
{
	std::vector<EditorSceneMeshData> meshes;
};

class EditorAssetLoader
{
public:
	enum FileType
	{
		FBX
	};

	static sptr<EditorSceneData> LoadSceneFromFile(std::string const& path, FileType type);
};

#endif // EDITOR_ASSET_LOADING_EDITOR_ASSET_LOADER_H__
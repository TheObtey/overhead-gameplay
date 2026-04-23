#ifndef EDITOR_ASSET_LOADING_EDITOR_ASSET_LOADER_H__
#define EDITOR_ASSET_LOADING_EDITOR_ASSET_LOADER_H__

#include <Define.h>
#include <GeometryFactory.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <map>

class Node;

struct EditorTextureRef
{
	uint32 textureType = 0;
	std::string path;
};

struct EditorSceneMeshData
{
	std::string name;
	uint32 ID;
	GeoInfo geometry;
	glm::mat4 meshMatrix = glm::mat4(1.0f);
	std::vector<EditorTextureRef> textures;
};

struct EditorNode
{
	std::string name;
	glm::mat4 transform = glm::mat4(1.0f);
	int parent = -1;
	int meshIndex = -1;
	std::vector<int> children;
};

struct EditorSceneData
{
	std::vector<EditorSceneMeshData> meshes;

	std::vector<EditorNode> nodes;
	int rootNode = -1;
};

class EditorAssetLoader
{
public:
	enum FileType
	{
		FBX
	};

	static sptr<EditorSceneData> LoadSceneFromFile(std::string const& path, FileType type);
	static uptr<Node> CreateNodesFromScene(EditorSceneData const& scene, std::string const& sourceFbxPath);

private:
	static uptr<Node> BuildNode(EditorSceneData const& scene, int index, std::string const& sourceFbxPath);

	inline static std::map<std::string, sptr<EditorSceneData>> m_loadedScenes = {};
};

#endif
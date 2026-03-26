#ifndef FOUNDRY_FBX_LOADER_H__
#define FOUNDRY_FBX_LOADER_H__

#include <string>
#include <map>
#include <vector>
#include <Define.h>
#include <glm/mat4x4.hpp>
#include <Texture.h>
#include <Geometry.h>


class aiNode;
class aiScene;
class FBXLoader
{
public:
	struct Node
	{
		std::string name;
		glm::mat4x4 transform = glm::mat4x4{ 1.0f };
		int32 parent = -1;
		std::vector<int32> children;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
		
		uint32 matIndex;
	};

	struct Material
	{
		std::map<TextureMaterialType,std::string> textures;
	};

	struct SceneData
	{
		std::vector<Node> nodes;
		std::vector<Mesh> meshs;
		std::vector<Material> textures;

	};

	static uptr<SceneData> LoadFile(std::string const& path);
private:
	static uint32 BuildNodes(aiNode const* pNode,int32 parentIndex, SceneData& outData);
	static void BuildMeshs(aiScene const* pScene, SceneData& outData);
	static void BuildMaterials(aiScene const* pScene, SceneData& outData);


	inline static std::unordered_map<std::string, SceneData> m_loadedFiles = {};
};


#endif
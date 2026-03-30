#ifndef FOUNDRY_FBX_LOADER_H__
#define FOUNDRY_FBX_LOADER_H__

#include "AssetLoading/AssetsStructs.h"

#include <Define.h>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <Texture.h>
#include <Geometry.h>

struct aiMesh;
struct aiScene;
struct aiNode;
struct aiAnimation;

class FBXLoader
{
public:

	struct Node
	{
		std::string name;
		glm::mat4x4 transform = glm::mat4x4{ 1.0f };
		int32 parent = -1;
		std::vector<int32> children;
		std::vector<uint32> meshesIndex;
	};

	struct FBXMesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint32> indices;
		std::vector<glm::mat4> bones;
		int32 matIndex = -1;
	};

	struct Material
	{
		std::map<TextureMaterialType,std::string> textures;
	};

	struct FBXSceneData
	{
		std::vector<sptr<Node>> nodes;
		std::vector<sptr<FBXMesh>> meshs;
		std::vector<Material> textures;
		std::vector<Light> lights;
		std::vector<Animation> animations;
	};

	static sptr<SceneData> LoadFile(std::string const& path);
private:
	static uint32 BuildNodes(aiScene const* pScene, aiNode const* pNode, int32 parentIndex, FBXSceneData& outData);
	static void BuildMeshs(aiScene const* pScene, FBXSceneData& outData);
	static void BuildMaterials(aiScene const* pScene, FBXSceneData& outData);
	static void BuildLights(aiScene const* pScene, FBXSceneData& outData);
	static void BuildBones(aiMesh const* pScene, FBXMesh& outData);
	static void BuildAnimations(aiScene const* pScene, FBXSceneData& outData);
	static void BuildAnimationsChannles(aiAnimation const* pAnim, Animation& outAnim, uint32 channelID);

	static void LoadTextures(FBXSceneData& outData,std::vector<sptr<Texture>>& vect, std::vector<Texture*>& tempVect, uint32 matIndex);
	static sptr<SceneData> ConvertInGlobalSceneData(FBXSceneData& outData);

private:
	static uint8 m_sTexTypes[];
	inline static std::unordered_map<std::string, sptr<SceneData>> m_loadedFiles = {};
};


#endif
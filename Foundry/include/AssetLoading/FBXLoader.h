#ifndef FOUNDRY_FBX_LOADER_H__
#define FOUNDRY_FBX_LOADER_H__

#include "AssetLoading/AssetsStructs.h"

#include <Define.h>
#include <Geometry.h>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>

struct aiMesh;
struct aiScene;
struct aiNode;
struct aiAnimation;

class FBXLoader
{
public:

	struct Material
	{
		std::vector<std::map<TextureMaterialType,std::string>> textures;
	};

	static sptr<SceneData> LoadFile(std::string const& path);

private:
	static uint32 BuildNodesTree(aiScene const* pScene, aiNode const* pNode, int32 parentIndex, SceneData& outData, Material& outMat);
	
	static void LoadEmbeddedTexture(std::string const& path, std::string& outPath, aiScene const* pScene, uint32& matIndex, TextureMaterialType type);
	static void BuildMaterials(aiScene const* pScene, Material& outMat);

	static void BuildGeometry(aiMesh const* pMesh,std::vector<Vertex>& vertices, std::vector<uint32>& indices);
	static void BuildBones(SceneData& outData, aiMesh const* pMesh, std::vector<Vertex>& vertices, std::vector<uint32>& indices, std::vector<glm::mat4>& bones);
	static void BuildMeshs(aiScene const* pScene, SceneData& outData, Material& outMat);


	static void BuildLights(aiScene const* pScene, SceneData& outData);
	
	static void BuildAnimations(aiScene const* pScene, SceneData& outData);
	static void BuildAnimationsChannles(aiAnimation const* pAnim, Animation& outAnim, uint32 channelID);


	static void LoadTextures(FBXLoader::Material& materials,std::vector<sptr<Texture>>& vect, std::vector<Texture*>& tempVect, uint32 matIndex);
	static void LoadDefaultsTextures(std::vector<sptr<Texture>>& vect, std::vector<Texture*>& tempVect);

private:
	static uint8 m_sTexTypes[];
	inline static std::unordered_map<std::string, sptr<SceneData>> m_loadedFiles = {};
};


#endif
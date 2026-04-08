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
		std::vector<std::map<Ore::TextureMaterialType,std::string>> textures;
	};

	static sptr<SceneData> LoadFile(std::string const& path);

private:
	static sptr<SceneNode> BuildNodesTree(aiScene const* pScene, aiNode const* pNode, int32 parentIndex, SceneData& outScene, Material& outMat);
	
	static void LoadEmbeddedTexture(std::string const& path, std::string& outPath, aiScene const* pScene, uint32& matIndex, Ore::TextureMaterialType type);
	static void BuildMaterials(aiScene const* pScene, Material& outMat);

	static void BuildGeometry(aiMesh const* pMesh,std::vector<Ore::Vertex>& vertices, std::vector<uint32>& indices);
	static void BuildBones(SceneData& outScene, aiMesh const* pMesh, SceneMesh& mesh);
	static void BuildMeshs(aiScene const* pScene, SceneData& outScene, Material& outMat);


	static void BuildLights(aiScene const* pScene, SceneData& outScene);
	
	static void BuildAnimations(aiScene const* pScene, SceneData& outScene);
	static void BuildAnimationsChannels(SceneData& outScene,aiAnimation const* pAnim, Animation& outAnim, uint32 channelID,uint8 ctrID, uint8 boneID);


	static void LoadTextures(FBXLoader::Material& materials,std::vector<sptr<Ore::Texture>>& vect, uint32 matIndex);
	static void LoadDefaultsTextures(std::vector<sptr<Ore::Texture>>& vect);
private:
	static uint8 m_sTexTypes[];
};


#endif
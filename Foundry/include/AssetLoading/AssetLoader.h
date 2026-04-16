#ifndef FOUNDRY_ASSET_LOADER_H__
#define FOUNDRY_ASSET_LOADER_H__

#include <Define.h>
#include <Expected.hpp>
#include "AssetLoading/AssetsStructs.h"

class Node;
class NodeMesh;
class Node3D;

class AssetLoader
{
public:
	enum FileType
	{
		FBX,
	};
	static sptr<SceneData> LoadSceneFromFile(std::string const& path, AssetLoader::FileType type);
	static uptr<Node> CreateNodesFromScene(SceneData const& scene);

private:
	static uptr<Node> LoadChild(SceneData const& scene, SceneNode& selfNode);
	static void LoadNode3D(Node3D& node, SceneNode& datas);
	//static void LoadAnimatedMesh(NodeMeshAnimated& root, SceneNode& selfNode);
};


#endif //! FOUNDRY_ASSET_LOADER_H__
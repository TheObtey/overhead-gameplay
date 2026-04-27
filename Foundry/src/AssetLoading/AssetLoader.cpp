#include "AssetLoading/AssetLoader.h"
#include "AssetLoading/FBXLoader.h"
#include "Nodes/Node3D.h"
#include "Nodes/NodeMesh.h"

#include <Mesh.h>
#include <Geometry.h>
#include <Logger.hpp>

sptr<SceneData> AssetLoader::LoadSceneFromFile(std::string const& path, AssetLoader::FileType type)
{
	if (AssetLoader::m_loadedScenes.contains(path))
		return m_loadedScenes[path];
	switch (type)
	{
	case AssetLoader::FBX:
		m_loadedScenes[path] = FBXLoader::LoadFile(path);
		return m_loadedScenes[path];
	default:
		Logger::LogWithLevel(LogLevel::ERROR, "Unable to load " + path + ". Extension File not supported");
		return nullptr;
	}
	Logger::LogWithLevel(LogLevel::ERROR, "File : " + path +" not loaded");
	return nullptr;
}

void AssetLoader::LoadNode3D(Node3D& node, SceneNode& datas)
{
	node.SetName(datas.name);
	// SetMatrix
}


uptr<Node> AssetLoader::LoadChild(SceneData const& scene, SceneNode& selfNode)
{
	uptr<Node> pNode = nullptr;
	SceneNode& node = selfNode;
	switch (node.type)
	{
	case MESH:
		pNode = Node::CreateNode<NodeMesh>("NewNode");
		dynamic_cast<NodeMesh*>(pNode.get())->SetFromSceneMesh(*scene.allMesh[node.MeshIndex],scene.path);
		break;
	case GLOBAL:
		pNode = Node::CreateNode<Node3D>("NewNode");
		AssetLoader::LoadNode3D(*dynamic_cast<Node3D*>(pNode.get()), node);
		break;
	default:
		break;
	}

	if (pNode == nullptr)
		return nullptr;

	for (uint32 i = 0; i < node.children.size();++i)
	{
		uptr<Node> newChild = LoadChild(scene, *node.children[i]);
		if (newChild != nullptr)
			pNode->AddChild(newChild);
	}
	return std::move(pNode);
}

uptr<Node> AssetLoader::CreateNodesFromScene(SceneData const& scene)
{

	return std::move(LoadChild(scene, *scene.rootNode));
}

sptr<Ore::Texture> AssetLoader::GetSharedTexture(std::string const& path, Ore::TextureMaterialType materialType)
{
	if (m_textureCache.contains(path))
		return m_textureCache[path];

	sptr<Ore::Texture> created = std::make_shared<Ore::Texture>(path, Ore::TextureType::TYPE_2D, materialType);
	m_textureCache[path] = created;
	return created;
}

std::string AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType type)
{
	return m_defaultTexturesPath[type];
}
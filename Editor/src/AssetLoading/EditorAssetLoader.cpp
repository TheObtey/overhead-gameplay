#include "AssetLoading/EditorAssetLoader.h"
#include "AssetLoading/EditorFBXLoader.h"

#include <Logger.hpp>

#include <Node.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeMesh.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace
{
	void ApplyMatrixToNode(Node3D& node, glm::mat4 const& matrix)
	{
		glm::vec3 scale(1.0f);
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		if (glm::decompose(matrix, scale, rotation, translation, skew, perspective))
		{
			node.SetLocalPosition(translation);
			node.SetScale(scale);
			node.SetLocalRotationQuat(glm::normalize(rotation));
		}
	}
}

sptr<EditorSceneData> EditorAssetLoader::LoadSceneFromFile(std::string const& path, FileType type)
{
	if (m_loadedScenes.contains(path))
		return m_loadedScenes[path];

	switch (type)
	{
	case FBX:
		m_loadedScenes[path] = EditorFBXLoader::LoadFile(path);
		return m_loadedScenes[path];
	default:
		Logger::LogWithLevel(LogLevel::ERROR, "[EditorAssetLoader] Unsupported file type: " + path);
		return nullptr;
	}
}

uptr<Node> EditorAssetLoader::BuildNode(EditorSceneData const& scene, int index, std::string const& fbxPath)
{
	auto const& n = scene.nodes[index];

	uptr<Node> node;



	if (n.meshIndex >= 0)
	{
		auto meshNode = Node::CreateNode<NodeMesh>(n.name);
		meshNode->SetFromEditorSceneMesh(
			scene.meshes[n.meshIndex],
			fbxPath
		);
		ApplyMatrixToNode(*meshNode, scene.meshes[n.meshIndex].meshMatrix);
		node = std::move(meshNode);
	}
	else
	{
		auto node3D = Node::CreateNode<Node3D>(n.name);
		ApplyMatrixToNode(*node3D, n.transform);
		node = std::move(node3D);
	}

	for (int child : n.children)
	{
		node->AddChild(BuildNode(scene, child, fbxPath));
	}

	return node;
}

uptr<Node> EditorAssetLoader::CreateNodesFromScene(EditorSceneData const& scene, std::string const& sourceFbxPath)
{
	if (scene.rootNode < 0)
		return nullptr;

	return BuildNode(scene, scene.rootNode, sourceFbxPath);
}
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
		glm::vec3 scale = glm::vec3(1.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 translation = glm::vec3(0.0f);
		glm::vec3 skew = glm::vec3(0.0f);
		glm::vec4 perspective = glm::vec4(0.0f);

		if (glm::decompose(matrix, scale, rotation, translation, skew, perspective))
		{
			node.SetLocalPosition(translation);
			node.SetScale(scale);
			node.SetLocalRotationQuat(glm::normalize(rotation));
		}
	}
}

sptr<EditorSceneData> EditorAssetLoader::LoadSceneFromFile(std::string const& path, EditorAssetLoader::FileType type)
{
	if (EditorAssetLoader::m_loadedScenes.contains(path))
		return m_loadedScenes[path];

	sptr<EditorSceneData> sptrScene = nullptr;
	switch (type)
	{
	case EditorAssetLoader::FBX:
		sptrScene = EditorFBXLoader::LoadFile(path);
		m_loadedScenes[path] = sptrScene;
		return sptrScene;
	default:
		Logger::LogWithLevel(LogLevel::ERROR, "[EditorAssetLoader] Unsupported file type for: " + path);
		return sptrScene;
	}
}

uptr<Node> EditorAssetLoader::CreateNodesFromScene(EditorSceneData const& scene, std::string const& sourceFbxPath)
{
	uptr<Node3D> root = Node::CreateNode<Node3D>("ImportedFBX");
	for (uint32 i = 0; i < static_cast<uint32>(scene.meshes.size()); ++i)
	{
		EditorSceneMeshData const& meshData = scene.meshes[i];

		uptr<NodeMesh> meshNode = Node::CreateNode<NodeMesh>("Mesh_" + std::to_string(i));
		meshNode->SetFbxPath(sourceFbxPath);

		ApplyMatrixToNode(*meshNode, meshData.meshMatrix);
		root->AddChild(std::move(meshNode));
	}

	return std::move(root);
}
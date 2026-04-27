#include "AssetLoading/EditorFBXLoader.h"

#include <Logger.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>
#include <fstream>
#include <functional>

glm::mat4 EditorFBXLoader::AIMatrixToGLMMatrix(aiMatrix4x4 const& matrix)
{
	glm::mat4 out;
	memcpy(&out, &matrix, sizeof(aiMatrix4x4));
	return glm::transpose(out);
}

namespace
{
	bool IsNearlyIdentity(glm::mat4 const& m, float eps = 1e-4f)
	{
		glm::mat4 id(1.0f);
		for (int c = 0; c < 4; ++c)
			for (int r = 0; r < 4; ++r)
				if (std::abs(m[c][r] - id[c][r]) > eps)
					return false;
		return true;
	}

	glm::mat4 ExtractTopLevelConverter(aiNode const* root)
	{
		glm::mat4 converter(1.0f);
		aiNode const* n = root;

		while (n)
		{
			bool converterNode = (n->mNumMeshes == 0) && (n->mNumChildren == 1);
			if (!converterNode) break;

			glm::mat4 local = EditorFBXLoader::AIMatrixToGLMMatrix(n->mTransformation);

			if (!IsNearlyIdentity(local))
				converter *= local;

			n = n->mChildren[0];
		}
		return converter;
	}
}

void EditorFBXLoader::BuildNodes(aiNode const* pNode, int parent, EditorSceneData& scene)
{
	EditorNode node;
	node.name = pNode->mName.C_Str();
	node.transform = AIMatrixToGLMMatrix(pNode->mTransformation);
	node.parent = parent;

	int index = scene.nodes.size();
	scene.nodes.push_back(node);

	if (parent >= 0)
		scene.nodes[parent].children.push_back(index);
	else
		scene.rootNode = index;

	if (pNode->mNumMeshes > 0)
		scene.nodes[index].meshIndex = pNode->mMeshes[0];

	for (uint32 i = 0; i < pNode->mNumChildren; ++i)
		BuildNodes(pNode->mChildren[i], index, scene);
}

void EditorFBXLoader::BuildMeshes(aiScene const* pScene, std::string const& sourcePath, EditorSceneData& outScene)
{
	std::vector<glm::mat4> meshWorldMatrices(pScene->mNumMeshes, glm::mat4(1.0f));
	std::vector<bool> assigned(pScene->mNumMeshes, false);

	std::function<void(aiNode const*, glm::mat4 const&)> collect;

	collect = [&](aiNode const* n, glm::mat4 const& parent)
		{
			glm::mat4 local = AIMatrixToGLMMatrix(n->mTransformation);
			glm::mat4 world = parent * local;

			for (uint32 i = 0; i < n->mNumMeshes; ++i)
			{
				uint32 idx = n->mMeshes[i];
				if (!assigned[idx])
				{
					meshWorldMatrices[idx] = world;
					assigned[idx] = true;
				}
			}

			for (uint32 i = 0; i < n->mNumChildren; ++i)
				collect(n->mChildren[i], world);
		};

	collect(pScene->mRootNode, glm::mat4(1.0f));

	glm::mat4 sceneFix(1.0f);
	glm::mat4 top = ExtractTopLevelConverter(pScene->mRootNode);
	if (!IsNearlyIdentity(top))
		sceneFix = glm::inverse(top);

	for (uint32 meshIndex = 0; meshIndex < pScene->mNumMeshes; ++meshIndex)
	{
		aiMesh* m = pScene->mMeshes[meshIndex];
		if (!m) continue;

		EditorSceneMeshData mesh;
		mesh.name = "Mesh_" + std::to_string(meshIndex);

		for (uint32 v = 0; v < m->mNumVertices; ++v)
		{
			Ore::Vertex vert{};
			vert.position = { m->mVertices[v].x, m->mVertices[v].y, m->mVertices[v].z };

			if (m->HasNormals())
				vert.normal = { m->mNormals[v].x, m->mNormals[v].y, m->mNormals[v].z };

			if (m->HasTextureCoords(0))
				vert.texCoords = { m->mTextureCoords[0][v].x, m->mTextureCoords[0][v].y };

			mesh.geometry.m_vertices.push_back(vert);
		}

		for (uint32 f = 0; f < m->mNumFaces; ++f)
		{
			for (uint32 i = 0; i < m->mFaces[f].mNumIndices; ++i)
				mesh.geometry.m_indices.push_back(m->mFaces[f].mIndices[i]);
		}

		mesh.meshMatrix = sceneFix * meshWorldMatrices[meshIndex];

		if (m->mMaterialIndex < pScene->mNumMaterials)
		{
			aiMaterial* mat = pScene->mMaterials[m->mMaterialIndex];

			aiTextureType types[] = {
				aiTextureType_DIFFUSE,
				aiTextureType_SPECULAR,
				aiTextureType_NORMALS,
				aiTextureType_HEIGHT,
				aiTextureType_OPACITY,
				aiTextureType_EMISSIVE,
				aiTextureType_AMBIENT
			};

			for (uint32 t = 0; t < 7; ++t)
			{
				if (mat->GetTextureCount(types[t]) == 0)
					continue;

				aiString path;
				if (mat->GetTexture(types[t], 0, &path) != AI_SUCCESS)
					continue;

				std::string raw = path.C_Str();
				std::string resolved;

				if (!raw.empty() && raw[0] == '*')
					resolved = ExtractEmbeddedTexture(pScene, raw, meshIndex, t);
				else
					resolved = ResolveTexturePath(sourcePath, raw);

				if (!resolved.empty())
					mesh.textures.push_back({ t, resolved });
			}
		}

		mesh.ID = outScene.meshes.size();
		if (!mesh.geometry.m_vertices.empty())
			outScene.meshes.push_back(std::move(mesh));
	}
}

sptr<EditorSceneData> EditorFBXLoader::LoadFile(std::string const& path)
{
	Assimp::Importer importer = Assimp::Importer();
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	int importFlags = aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GlobalScale;

	aiScene const* pAScene = importer.ReadFile(path.c_str(), importFlags);
	if (pAScene == nullptr)
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Failed Loading " + path + " file");
		return nullptr;
	}

	auto result = std::make_shared<EditorSceneData>();

	BuildNodes(pAScene->mRootNode, -1, *result);
	BuildMeshes(pAScene, path, *result);

	return result;
}

std::string EditorFBXLoader::ResolveTexturePath(std::string const& sourcePath, std::string const& texturePath)
{
	if (texturePath.empty())
		return {};

	std::filesystem::path tex(texturePath);
	if (tex.is_absolute() && std::filesystem::exists(tex))
		return tex.generic_string();

	std::filesystem::path fbxDir = std::filesystem::path(sourcePath).parent_path();
	std::filesystem::path besideFbx = fbxDir / tex;
	if (std::filesystem::exists(besideFbx))
		return besideFbx.generic_string();

	std::filesystem::path inGameTextures = std::filesystem::path("../Game/res/textures") / tex.filename();
	if (std::filesystem::exists(inGameTextures))
		return inGameTextures.generic_string();

	return {};
}

std::string EditorFBXLoader::ExtractEmbeddedTexture(aiScene const* pScene, std::string const& texturePath, uint32 meshIndex, uint32 type)
{
	if (texturePath.empty() || texturePath[0] != '*')
		return {};

	aiTexture const* pTex = pScene->GetEmbeddedTexture(texturePath.c_str());
	if (!pTex || pTex->mHeight != 0)
		return {};

	std::string ext = pTex->achFormatHint;
	if (ext.empty()) ext = "bin";

	std::filesystem::path outDir = "res/textures";
	std::filesystem::create_directories(outDir);

	std::filesystem::path outPath = outDir / ("editor_embedded_" + std::to_string(meshIndex) + "_" + std::to_string(type) + "." + ext);

	std::ofstream file(outPath, std::ios::binary);
	if (!file.is_open())
		return {};

	file.write(reinterpret_cast<char const*>(pTex->pcData), static_cast<std::streamsize>(pTex->mWidth));
	file.close();

	return outPath.generic_string();
}
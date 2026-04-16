#include "AssetLoading/EditorFBXLoader.h"

#include <Logger.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <fstream>
#include <functional>

glm::mat4 EditorFBXLoader::AIMatrixToGLMMatrix(aiMatrix4x4 const& matrix)
{
	glm::mat4 out = {};
	memcpy(&out, &matrix, sizeof(aiMatrix4x4));
	return glm::transpose(out);
}

namespace
{
	bool IsNearlyIdentity(glm::mat4 const& m, float eps = 1e-4f)
	{
		glm::mat4 const id(1.0f);
		for (int c = 0; c < 4; ++c)
		{
			for (int r = 0; r < 4; ++r)
			{
				if (std::abs(m[c][r] - id[c][r]) > eps) return false;
			}
		}
		return true;
	}

	glm::mat4 ExtractTopLevelConverter(aiNode const* root, auto&& toGlm)
	{
		glm::mat4 converter(1.0f);
		aiNode const* n = root;

		while (n)
		{
			bool const converterNode = (n->mNumMeshes == 0) && (n->mNumChildren == 1);
			if (!converterNode) break;

			glm::mat4 const local = toGlm(n->mTransformation);
			if (!IsNearlyIdentity(local))
			{
				converter = converter * local;
			}

			n = n->mChildren[0];
		}

		return converter;
	}
}

void EditorFBXLoader::BuildMeshes(aiScene const* pScene, std::string const& sourcePath, EditorSceneData& outScene)
{
	std::vector<glm::mat4> meshWorldMatrices(pScene->mNumMeshes, glm::mat4(1.0f));
	std::vector<bool> meshMatrixAssigned(pScene->mNumMeshes, false);

	std::function<void(aiNode const*, glm::mat4 const&)> collectNodeTransforms;

	collectNodeTransforms = [&](aiNode const* pNode, glm::mat4 const& parentWorld)
	{
		if (!pNode) return;

		glm::mat4 const local = AIMatrixToGLMMatrix(pNode->mTransformation);
		glm::mat4 const world = parentWorld * local;

		for (uint32 i = 0; i < pNode->mNumMeshes; ++i)
		{
			uint32 const meshIdx = pNode->mMeshes[i];
			if (meshIdx < meshWorldMatrices.size() && !meshMatrixAssigned[meshIdx])
			{
				meshWorldMatrices[meshIdx] = world;
				meshMatrixAssigned[meshIdx] = true;
			}
		}

		for (uint32 c = 0; c < pNode->mNumChildren; ++c)
		{
			collectNodeTransforms(pNode->mChildren[c], world);
		}
	};

	collectNodeTransforms(pScene->mRootNode, glm::mat4(1.0f));

	glm::mat4 sceneFix(1.0f);
	if (pScene->mRootNode)
	{
		glm::mat4 const topConverter = ExtractTopLevelConverter(
			pScene->mRootNode,
			[](aiMatrix4x4 const& m) { return EditorFBXLoader::AIMatrixToGLMMatrix(m); });

		if (!IsNearlyIdentity(topConverter))
		{
			sceneFix = glm::inverse(topConverter);
		}
	}

	for (uint32 meshIndex = 0; meshIndex < pScene->mNumMeshes; ++meshIndex)
	{
		aiMesh* pMesh = pScene->mMeshes[meshIndex];
		if (!pMesh) continue;

		EditorSceneMeshData meshData;
		meshData.geometry.m_vertices.reserve(pMesh->mNumVertices);
		meshData.geometry.m_indices.reserve(pMesh->mNumFaces * 3);

		for (uint32 v = 0; v < pMesh->mNumVertices; ++v)
		{
			Ore::Vertex vertex = {};
			vertex.position = { pMesh->mVertices[v].x, pMesh->mVertices[v].y, pMesh->mVertices[v].z };

			if (pMesh->HasNormals())
				vertex.normal = { pMesh->mNormals[v].x, pMesh->mNormals[v].y, pMesh->mNormals[v].z };

			if (pMesh->HasTextureCoords(0))
				vertex.texCoords = { pMesh->mTextureCoords[0][v].x, pMesh->mTextureCoords[0][v].y };

			meshData.geometry.m_vertices.push_back(vertex);
		}

		for (uint32 f = 0; f < pMesh->mNumFaces; ++f)
		{
			aiFace const& face = pMesh->mFaces[f];
			for (uint32 i = 0; i < face.mNumIndices; ++i)
				meshData.geometry.m_indices.push_back(face.mIndices[i]);
		}
		meshData.meshMatrix = sceneFix * meshWorldMatrices[meshIndex];
		if (pMesh->mMaterialIndex < pScene->mNumMaterials)
		{
			aiMaterial* pMat = pScene->mMaterials[pMesh->mMaterialIndex];
			if (pMat)
			{
				aiTextureType const types[] = {
					aiTextureType_DIFFUSE,
					aiTextureType_SPECULAR,
					aiTextureType_NORMALS,
					aiTextureType_HEIGHT,
					aiTextureType_OPACITY,
					aiTextureType_EMISSIVE,
					aiTextureType_AMBIENT
				};

				for (aiTextureType t : types)
				{
					uint32 count = pMat->GetTextureCount(t);
					for (uint32 ti = 0; ti < count; ++ti)
					{
						aiString path;
						if (pMat->GetTexture(t, ti, &path) != AI_SUCCESS)
							continue;

						std::string resolved;
						std::string raw = path.C_Str();

						if (!raw.empty() && raw[0] == '*')
							resolved = ExtractEmbeddedTexture(pScene, raw, meshIndex, static_cast<uint32>(t));
						else
							resolved = ResolveTexturePath(sourcePath, raw);

						if (!resolved.empty())
							meshData.textures.push_back({ static_cast<uint32>(t), resolved });
					}
				}
			}
		}

		if (!meshData.geometry.m_vertices.empty() && !meshData.geometry.m_indices.empty())
			outScene.meshes.push_back(std::move(meshData));
	}
}

sptr<EditorSceneData> EditorFBXLoader::LoadFile(std::string const& path)
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

	int const importFlags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GlobalScale;

	aiScene const* pScene = importer.ReadFile(path.c_str(), importFlags);
	if (!pScene)
	{
		Logger::LogWithLevel(LogLevel::ERROR, "[EditorFBXLoader] Failed loading: " + path);
		return nullptr;
	}

 	sptr<EditorSceneData> result = std::make_shared<EditorSceneData>();
	BuildMeshes(pScene, path, *result);

	if (result->meshes.empty())
	{
		Logger::LogWithLevel(LogLevel::WARNING, "[EditorFBXLoader] No valid mesh in: " + path);
	}

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
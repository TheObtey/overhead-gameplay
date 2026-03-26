#include "AssetLoading/AssetLoader.h"
#include "AssetLoading/FBXLoader.h"

#include <Mesh.h>
#include <Define.h>
#include <Geometry.h>

std::vector<Mesh*> AssetLoader::LoadMeshFromFile(std::string const& path, FileType type)
{
	switch (type)
	{
	case AssetLoader::FBX:
		return LoadMeshFromFBX(path);
	default:
		//Logger::LogWithLevel(LogLevel::ERROR, "Unable to load " + path + ". Extension File not supported");
		return std::vector<Mesh*>();
	}
}

std::vector<Mesh*> AssetLoader::LoadMeshFromFBX(std::string const& path)
{
	uptr<FBXLoader::SceneData> fbxScene = FBXLoader::LoadFile(path);
	std::vector<Mesh*> outVect = {};

	for (uint32 meshCount = 0; meshCount < fbxScene->meshs.size(); ++meshCount)
	{
		Geometry geo = Geometry(fbxScene->meshs[meshCount].vertices, fbxScene->meshs[meshCount].indices);
		FBXLoader::Material meshMat = fbxScene->textures[fbxScene->meshs[meshCount].matIndex];
		std::vector<Texture*> texts = {};
		for (std::map<TextureMaterialType, std::string>::iterator it = meshMat.textures.begin(); it != meshMat.textures.end(); ++it)
		{
			sptr<Texture> text = std::make_shared<Texture>(Texture(it->second, TextureType::TYPE_2D, it->first));
			texts.push_back(text.get());
		}
		sptr<Mesh> newMesh = std::make_shared<Mesh>(Mesh(geo, texts, glm::mat4{ 1.0f }));
		outVect.push_back(newMesh.get());
	}

	return outVect;
}


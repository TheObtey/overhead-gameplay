#include "AssetLoading/AssetLoader.h"
#include "AssetLoading/FBXLoader.h"

#include <Mesh.h>
#include <Define.h>
#include <Geometry.h>
#include <Logger.hpp>

std::vector<Mesh*> AssetLoader::LoadMeshFromFile(std::string const& path, AssetLoader::FileType type)
{
	switch (type)
	{
	case AssetLoader::FBX:
		return LoadMeshFromFBX(path);
	default:
		Logger::LogWithLevel(LogLevel::ERROR, "Unable to load " + path + ". Extension File not supported");
		return std::vector<Mesh*>();
	}

	return std::vector<Mesh*>();
}


std::vector<Mesh*> AssetLoader::LoadMeshFromFBX(std::string const& path)
{
	uptr<FBXLoader::SceneData> fbxScene = FBXLoader::LoadFile(path);
	std::vector<Mesh*> outVect = {};

	for (uint32 meshCount = 0; meshCount < fbxScene->meshs.size(); ++meshCount)
	{
		Geometry geo = Geometry(fbxScene->meshs[meshCount].vertices, fbxScene->meshs[meshCount].indices);
		std::vector<Texture*> texts = {};
		
		FBXLoader::Material meshMat = {};
		if (fbxScene->textures.size() != 0)
		{
			meshMat = fbxScene->textures[fbxScene->meshs[meshCount].matIndex];
			for (std::map<TextureMaterialType, std::string>::iterator it = meshMat.textures.begin(); it != meshMat.textures.end(); ++it)
			{
				sptr<Texture> text = std::make_shared<Texture>(Texture(it->second, TextureType::TYPE_2D, it->first));
				texts.push_back(text.get());
			}
		}

		if (fbxScene->textures.size() == 0 || meshMat.textures.size() == 0)
		{
			Texture* text = new Texture(Texture("res/textures/NormalMap.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL));
			texts.push_back(text);
			Texture* text2 = new Texture(Texture("res/textures/diffuse.jpg", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE));
			texts.push_back(text2);
			Texture* text3 = new Texture(Texture("res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR));
			texts.push_back(text3);
		}

		Mesh* newMesh = new Mesh(Mesh(geo, texts, glm::mat4{ 1.0f }));
		outVect.push_back(newMesh);
	}

	return outVect;
}


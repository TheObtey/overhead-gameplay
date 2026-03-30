#include "AssetLoading/AssetLoader.h"
#include "AssetLoading/FBXLoader.h"

#include <Mesh.h>
#include <Geometry.h>
#include <Logger.hpp>

sptr<SceneData> AssetLoader::LoadSceneFromFile(std::string const& path, AssetLoader::FileType type)
{
	switch (type)
	{
	case AssetLoader::FBX:
		return FBXLoader::LoadFile(path);
	default:
		Logger::LogWithLevel(LogLevel::ERROR, "Unable to load " + path + ". Extension File not supported");
		return nullptr;
	}
	Logger::LogWithLevel(LogLevel::ERROR, "File : " + path +" not loaded");
	return nullptr;
}


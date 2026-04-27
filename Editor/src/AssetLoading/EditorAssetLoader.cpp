#include "AssetLoading/EditorAssetLoader.h"
#include "AssetLoading/EditorFBXLoader.h"

#include <Logger.hpp>

sptr<EditorSceneData> EditorAssetLoader::LoadSceneFromFile(std::string const& path, EditorAssetLoader::FileType type)
{
	switch (type)
	{
	case EditorAssetLoader::FBX:

		return EditorFBXLoader::LoadFile(path);
	default:
		Logger::LogWithLevel(LogLevel::ERROR, "[EditorAssetLoader] Unsupported file type for: " + path);
		return nullptr;
	}
}
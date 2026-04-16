#include "AssetLoading/EditorAssetLoader.h"
#include "AssetLoading/EditorFBXLoader.h"

#include <Logger.hpp>

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
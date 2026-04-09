#ifndef FOUNDRY_ASSET_LOADER_H__
#define FOUNDRY_ASSET_LOADER_H__

#include <Define.h>
#include <Expected.hpp>
#include "AssetLoading/AssetsStructs.h"

class AssetLoader
{
public:
	enum FileType
	{
		FBX,
	};
	static sptr<SceneData> LoadSceneFromFile(std::string const& path, AssetLoader::FileType type);
	
};


#endif //! FOUNDRY_ASSET_LOADER_H__
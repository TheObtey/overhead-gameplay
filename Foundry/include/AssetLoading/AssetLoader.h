#ifndef FOUNDRY_ASSET_LOADER_H__
#define FOUNDRY_ASSET_LOADER_H__

#include <string>
#include <vector>

class Mesh;

class AssetLoader
{
public:
	enum FileType
	{
		FBX,
	};
	static std::vector<Mesh*> LoadMeshFromFile(std::string const& path, AssetLoader::FileType type);
	
private:
	static std::vector<Mesh*> LoadMeshFromFBX(std::string const& path);
};


#endif //! FOUNDRY_ASSET_LOADER_H__
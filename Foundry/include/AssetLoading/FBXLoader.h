#ifndef FOUNDRY_ASSET_LOADER_H__
#define FOUNDRY_ASSET_LOADER_H__

// need struct of Objects
#include <string>
#include <Define.h>
#include <glm/mat4x4.hpp>

class aiNode;

struct Node
{
	std::string name;
	glm::mat4x4 transform;
	int32 parent;
	std::vector<int32> children;
};
	
struct SceneData
{
	std::vector<Node> nodes;
};

class FBXLoader
{
public:
	static uptr<SceneData> LoadFile(std::string const& path);
private:
	static uint32 BuildNodes(aiNode const* pNode,int32 parentIndex, SceneData& outData);

};


#endif
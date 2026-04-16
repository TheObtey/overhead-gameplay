#ifndef FOUNDRY_ASSET_STRUCT_H__
#define FOUNDRY_ASSET_STRUCT_H__

#include "AnimationStructs.h"

#include <Define.h>
#include <vector>
#include <string>
#include <Mesh.h>
#include <Texture.h>
#include <Passes/LightPass.h>
#include <glm/glm.hpp>


enum SceneNodeType
{
	GLOBAL,
	MESH,
};

struct SceneNode
{
	SceneNodeType type = GLOBAL;

	std::string name;
	glm::mat4 transform;

	int32 boneIndexInMesh = -1;
	int32 parent = -1;
	int32 MeshIndex = -1;
	bool isCtrlGrp = false;
	std::vector<sptr<SceneNode>> children;
};

struct SceneMesh
{
	// Geometry
	std::string name;
 	std::vector<Ore::Vertex> vertices;
	std::vector<uint32> indices;

	std::vector<sptr<Ore::Texture>> meshTextures;
	std::vector<glm::mat4> bonesOffest;
	std::vector<glm::mat4> bonesTransform;
	glm::mat4 meshMatrix;
	std::vector<uint32> animationlinked;
};

struct SceneData
{
	sptr<SceneNode> rootNode;
	std::filesystem::path path;
	bool isAnimated = false;
	std::vector<sptr<SceneNode>> allNode;
	std::vector<sptr<Ore::Light>> alllights;
	std::vector<sptr<Animation>> animations;
	std::vector<sptr<SceneMesh>> allMesh;
};

#endif // !FOUNDRY_ASSET_STRUCT_H__

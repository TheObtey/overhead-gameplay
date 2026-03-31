#ifndef FOUNDRY_ASSET_STRUCT_H__
#define FOUNDRY_ASSET_STRUCT_H__

#include <Define.h>
#include <vector>
#include <string>
#include <Mesh.h>
#include <Texture.h>
#include <Passes/LightPass.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


enum SceneNodeType
{
	GLOBAL,
	MESH,
	BONE,
};

struct SceneNode
{
	SceneNodeType type = GLOBAL;

	std::string name;
	glm::mat4 transform;

	int32 boneIndexInMesh = -1;
	int32 parent = -1;
	std::vector<uint32> meshesIndex;
	std::vector<uint32> children;
};

struct AnimationChannel
{
	uint32 sceneNodeImpacted;
	struct FrameVec3
	{
		float time;
		glm::vec3 vec;
	};
	struct FrameQuat
	{
		float time;
		glm::quat quaternion;
	};
	std::vector<FrameVec3> positionKeys;
	std::vector<FrameQuat> rotationKeys;
	std::vector<FrameVec3> scalingKeys;
};

struct Animation
{
	std::string name;
	float duration;
	std::vector<AnimationChannel> animationTransform;
};


struct SceneData
{

	sptr<SceneNode> rootNode;

	bool isAnimated = false;
	std::vector<sptr<SceneNode>> allNode;
	std::vector<Animation> animations;
	std::vector<Light> alllights;
	std::vector<sptr<Mesh>> allMesh;
	std::vector<sptr<Texture>> allTextures;
};

#endif // !FOUNDRY_ASSET_STRUCT_H__

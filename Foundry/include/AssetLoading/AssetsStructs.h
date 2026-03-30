#ifndef FOUNDRY_ASSET_STRUCT_H__
#define FOUNDRY_ASSET_STRUCT_H__

#include <vector>
#include <string>
#include <Mesh.h>
#include <Texture.h>
#include <Passes/LightPass.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct AnimationChannel
{
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

struct SceneMeshs
{
	sptr<Mesh> mesh;
	std::vector<sptr<Texture>> textureOfMeshes;
};

struct SceneData
{
	std::vector<SceneMeshs> meshes;
	std::vector<Light> lights;
	std::vector<Animation> animations;
};

#endif // !FOUNDRY_ASSET_STRUCT_H__

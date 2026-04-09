#ifndef FOUNDRY_ANIMATION_STRUCT_H__
#define FOUNDRY_ANIMATION_STRUCT_H__

#include <Define.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct AnimationChannel
{
	uint32 sceneNodeImpacted;
	uint32 frameCount;
	uint32 currentFrame;
	bool isController;
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
	glm::mat4 originalWT;
};

struct Animation
{
	std::string name;
	bool isRestarting;
	bool isLooping;
	float currentTime;
	float duration;
	std::vector<sptr<AnimationChannel>> animationTransform;
};

#endif // !FOUNDRY_ANIMATION_STRUCT_H__

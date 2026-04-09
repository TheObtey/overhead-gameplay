#ifndef MATHSUTILS__H_
#define MATHSUTILS__H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Maths
{
	static glm::mat4x4 Translate(const glm::vec3& pos) {
		return glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			pos.x, pos.y, pos.z, 1
		);
	}

	static glm::mat4x4 Scale(const glm::vec3& scale) {
		return glm::mat4x4(
			scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			0, 0, 0, 1
		);
	}

	static glm::mat4x4 RotateYawPitchRoll(const glm::vec4& YawPitchRoll) {
		glm::quat q = glm::angleAxis(YawPitchRoll.x, glm::vec3{ 0.0f, 1.0f, 0.0f });

		glm::vec3 right = { 1.f, 0.f, 0.0f };
		glm::vec3 forward = { 0.f, 0.f, 1.0f };

		right = q * right;
		q = glm::angleAxis(YawPitchRoll.y, right) * q;

		forward = q * forward;
		q = glm::angleAxis(YawPitchRoll.z, forward) * q;

		return glm::mat4_cast(q);
	}

	// /!\/!\  Utiliser  glm::mix pour les vecteurs et glm::lerp ou glm::slerp pour les quaternions  /!\/!\  

	//static glm::vec3 Lerp(glm::vec3 v1, glm::vec3 v2, float ratio)
	//{
	//	return { std::lerp(v1.x, v2.x, ratio), std::lerp(v1.y, v2.y, ratio), std::lerp(v1.z, v2.z, ratio) };
	//}

	//static glm::vec4 Lerp(glm::vec4 v1, glm::vec4 v2, float ratio)
	//{
	//	return { std::lerp(v1.x, v2.x, ratio), std::lerp(v1.y, v2.y, ratio), std::lerp(v1.z, v2.z, ratio), std::lerp(v1.w, v2.w, ratio) };
	//}

	//static glm::quat Slerp(glm::quat q1, glm::quat q2, float ratio)
	//{
	//	float d = glm::dot(q1, q2);
	//	if (d < 0)
	//	{
	//		q2 = -q2;
	//		d = -d;
	//	}
	//	if (d > 0.9995) return glm::normalize(glm::mix(q1, q2, ratio));
	//	float theta = std::acos(d);
	//	float s = std::sin(theta);
	//	return (std::sin((1.f - ratio) * theta) / s) * q1 + (std::sin(ratio * theta) / s) * q2;
	//}
}

#endif

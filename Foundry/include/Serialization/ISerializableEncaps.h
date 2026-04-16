#ifndef _ISERIALIZABLE_ENCAPS_H__
#define _ISERIALIZABLE_ENCAPS_H__

#include "Serialization/ISerializable.h"
#include "Registries/AutomaticRegister.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <algorithm>

class SVec2 : public ISerializable
{
public:
	SVec2(glm::vec2 const& vec);

	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	float x, y;
};
REGISTER_ISERIALIZABLE(SVec2, SVec2::CreateInstance);

class SVec3 : public ISerializable
{
public:
	SVec3(glm::vec3 const& vec);

	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	float x, y, z;
};
REGISTER_ISERIALIZABLE(SVec3, SVec3::CreateInstance);

class SVec4 : public ISerializable
{
public:
	SVec4(glm::vec4 const& vec);

	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	float x, y, z, w;
};
REGISTER_ISERIALIZABLE(SVec4, SVec4::CreateInstance);


class ClampedInt : public ISerializable
{
public:
	ClampedInt(int min, int max, int value);

	void SetMin(int _min) { min = _min; }
	void SetMax(int _max) { max = _max; }
	void SetValue(int _val);

	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	int min,max, value;
};
REGISTER_ISERIALIZABLE(ClampedInt, ClampedInt::CreateInstance);

class ClampedFloat : public ISerializable
{
public:
	ClampedFloat(float min, float max, float value);

	void SetMin(float _min) { min = _min; }
	void SetMax(float _max) { max = _max; }
	void SetValue(float _val);


	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	float min, max, value;
};
REGISTER_ISERIALIZABLE(ClampedFloat, ClampedFloat::CreateInstance);

#endif // !_ISERIALIZABLE_ENCAPS_H__

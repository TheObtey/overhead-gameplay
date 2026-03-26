#ifndef _ISERIALIZABLE_ENCAPS_H__
#define _ISERIALIZABLE_ENCAPS_H__

#include "Serialization/ISerializable.h"
#include "Registries/AutomaticRegister.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class SVec2 : public ISerializable
{
public:
	SVec2(glm::vec2 const& vec);

	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	float x, y;
};
inline REGISTER_ISERIALIZABLE(SVec2, SVec2::CreateInstance);

class SVec3 : public ISerializable
{
public:
	SVec3(glm::vec3 const& vec);

	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	float x, y, z;
};
inline REGISTER_ISERIALIZABLE(SVec3, SVec3::CreateInstance);

class SVec4 : public ISerializable
{
public:
	SVec4(glm::vec4 const& vec);

	void Serialize(SerializedObject& datas) const override;
	void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

	float x, y, z, w;
};
inline REGISTER_ISERIALIZABLE(SVec4, SVec4::CreateInstance);

#endif // !_ISERIALIZABLE_ENCAPS_H__

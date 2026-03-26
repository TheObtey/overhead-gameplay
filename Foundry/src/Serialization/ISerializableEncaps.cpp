
#include <Serialization/ISerializableEncaps.h>
#include <Serialization/SerializeObject.hpp>

// VEC2
SVec2::SVec2(glm::vec2 const& vec):	x(vec.x), y(vec.y)
{}

void SVec2::Serialize(SerializedObject& datas) const
{
	datas.SetType("SVec2");
	datas.AddPublicElement("x", &x);
	datas.AddPublicElement("y", &y);
}
void SVec2::Deserialize(SerializedObject const& datas)
{
	datas.GetPublicElement("x", &x);
	datas.GetPublicElement("y", &y);
}

ISerializable* SVec2::CreateInstance()
{
	return static_cast<ISerializable*>(std::make_unique<SVec2>(glm::vec2()).release());
}

// VEC3
SVec3::SVec3(glm::vec3 const& vec) : x(vec.x), y(vec.y), z(vec.z)
{
}

void SVec3::Serialize(SerializedObject& datas) const
{
	datas.SetType("SVec3");
	datas.AddPublicElement("x", &x);
	datas.AddPublicElement("y", &y);
	datas.AddPublicElement("z", &z);
}
void SVec3::Deserialize(SerializedObject const& datas)
{
	datas.GetPublicElement("x", &x);
	datas.GetPublicElement("y", &y);
	datas.GetPublicElement("z", &z);
}

ISerializable* SVec3::CreateInstance()
{
	return static_cast<ISerializable*>(std::make_unique<SVec3>(glm::vec3()).release());
}

// VEC4
SVec4::SVec4(glm::vec4 const& vec) : x(vec.x), y(vec.y), z(vec.z),w(vec.w)
{
}

void SVec4::Serialize(SerializedObject& datas) const
{
	datas.SetType("SVec4");
	datas.AddPublicElement("x", &x);
	datas.AddPublicElement("y", &y);
	datas.AddPublicElement("z", &z);
	datas.AddPublicElement("w", &w);
}
void SVec4::Deserialize(SerializedObject const& datas)
{
	datas.GetPublicElement("x", &x);
	datas.GetPublicElement("y", &y);
	datas.GetPublicElement("z", &z);
	datas.GetPublicElement("w", &w);
}

ISerializable* SVec4::CreateInstance()
{
	return static_cast<ISerializable*>(std::make_unique<SVec4>(glm::vec4()).release());
}

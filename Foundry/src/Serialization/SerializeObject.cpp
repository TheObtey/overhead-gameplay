#include "Serialization/SerializeObject.hpp"

SerializedObject::SerializedObject()
{
	m_elementsInSerializedObject = json::object();
	m_elementsInSerializedObject["PRIVATE_DATAS"] = json::object();
	m_elementsInSerializedObject["PUBLIC_DATAS"] = json::object();
}

void SerializedObject::SetType(std::string const& className)
{
	m_elementsInSerializedObject["PRIVATE_DATAS"]["TYPE"] = className;
}

std::string SerializedObject::GetType() const
{
	return m_elementsInSerializedObject["PRIVATE_DATAS"]["TYPE"];
}

void SerializedObject::AddPrivateArray(std::string const& arrayName)
{
	m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName] = json::array();
}

void SerializedObject::AddPrivateDictionary(std::string const& dictionnaryName)
{
	m_elementsInSerializedObject["PRIVATE_DATAS"][dictionnaryName] = json::object();
}

void SerializedObject::AddPublicArray(std::string const& arrayName)
{
	m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName] = json::array();
}
void SerializedObject::AddPublicDictionary(std::string const& dictionnaryName)
{
	m_elementsInSerializedObject["PUBLIC_DATAS"][dictionnaryName] = json::object();
}
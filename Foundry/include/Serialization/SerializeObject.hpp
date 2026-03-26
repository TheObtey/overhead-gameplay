#ifndef _SERIALIZEOBJECT__H
#define _SERIALIZEOBJECT__H

#include "ISerializable.h"
#include "json.hpp"
#include "Registries/AutomaticRegister.hpp"
#include "Serialization/ISerializableEncaps.h"

#include <Define.h>
#include <Logger.hpp>
#include <string>

using json = nlohmann::json;

/*
	* Object used to convert Data of an object in json
*/
class SerializedObject
{
public:
	SerializedObject();

	void SetType(std::string const& className);

	std::string GetType() const;
	
	// Public
	void AddPrivateArray(std::string const& arrayName);
	void AddPrivateDictionary(std::string const& dictionaryName);

	template <typename T>
	void AddPrivateElement(std::string const& variableName, T const* variableData);

	template <typename T>
	void AddPrivateElementInDictionary(std::string const& disctionnaryName, std::string const& name, T const* element);

	template <typename T>
	void AddPrivateElementInArray(std::string const& arrayName,T const* element);

	template <typename T>
	void GetPrivateElement(std::string const& elementName, T* outVariable) const;

	template <typename T>
	std::vector<T> GetPrivateArray(std::string const& arrayName) const;

	template <typename T>
	void GetPrivateElementInDictionary(std::string const& dictionaryName, std::string const& elementName, T* outVariable) const;

	// PUBLIC
	void AddPublicArray(std::string const& arrayName);
	void AddPublicDictionary(std::string const& dictionaryName);

	template <typename T>
	void AddPublicElement(std::string const& variableName, T const* variableData);

	template <typename T>
	void AddPublicElementInDictionary(std::string const& disctionnaryName, std::string const& name, T const* element);

	template <typename T>
	void AddPublicElementInArray(std::string const& arrayName, T const* element);

	template <typename T>
	void GetPublicElement(std::string const& elementName, T* outVariable) const;

	template <typename T>
	std::vector<T> GetPublicArray(std::string const& arrayName) const;

	template <typename T>
	void GetPublicElementInDictionary(std::string const& dictionaryName, std::string const& elementName, T* outVariable) const;


	json const& GetJson() {return m_elementsInSerializedObject;}
	void SetJson(json const& json) { m_elementsInSerializedObject = json; } // Check d'error

private:
	json m_elementsInSerializedObject;
	friend class EditorSerializer;
};

// PRIVATE ELEMENT
template <typename T>
inline void SerializedObject::AddPrivateElement(std::string const& variableName, T const* variableData)
{
	m_elementsInSerializedObject["PRIVATE_DATAS"][variableName] = *variableData;
}
template <>
inline void SerializedObject::AddPrivateElement<ISerializable>(std::string const& variableName, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PRIVATE_DATAS"][variableName] = object.m_elementsInSerializedObject;
}
template <>
inline void SerializedObject::AddPrivateElement<glm::vec2>(std::string const& variableName, glm::vec2 const* variableData)
{
	SVec2 sVar = SVec2(*variableData);
	AddPrivateElement(variableName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPrivateElement<glm::vec3>(std::string const& variableName, glm::vec3 const* variableData)
{
	SVec3 sVar = SVec3(*variableData);
	AddPrivateElement(variableName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPrivateElement<glm::vec4>(std::string const& variableName, glm::vec4 const* variableData)
{
	SVec4 sVar = SVec4(*variableData);
	AddPrivateElement(variableName, static_cast<ISerializable const*>(&sVar));
}

// PRIVATE Dictionary
template <typename T>
inline void SerializedObject::AddPrivateElementInDictionary(std::string const& disctionnaryName, std::string const& name, T const* element)
{
	m_elementsInSerializedObject["PRIVATE_DATAS"][disctionnaryName][name] = *element;
}
template <>
inline void SerializedObject::AddPrivateElementInDictionary<ISerializable>(std::string const& disctionnaryName, std::string const& name, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PRIVATE_DATAS"][disctionnaryName][name] = object.m_elementsInSerializedObject;
}
template <>
inline void SerializedObject::AddPrivateElementInDictionary<glm::vec2>(std::string const& disctionnaryName, std::string const& name, glm::vec2 const* element)
{
	SVec2 sVar = SVec2(*element);
	AddPrivateElementInDictionary(disctionnaryName, name, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPrivateElementInDictionary<glm::vec3>(std::string const& disctionnaryName, std::string const& name, glm::vec3 const* element)
{
	SVec3 sVar = SVec3(*element);
	AddPrivateElementInDictionary(disctionnaryName, name, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPrivateElementInDictionary<glm::vec4>(std::string const& disctionnaryName, std::string const& name, glm::vec4 const* element)
{
	SVec4 sVar = SVec4(*element);
	AddPrivateElementInDictionary(disctionnaryName, name, static_cast<ISerializable const*>(&sVar));
}

// PRIVATE ARRAY
template <typename T>
inline void SerializedObject::AddPrivateElementInArray(std::string const& arrayName, T const* element)
{
	m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].push_back(*element);
}
template <>
inline void SerializedObject::AddPrivateElementInArray<ISerializable>(std::string const& arrayName, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].push_back(object.m_elementsInSerializedObject);
}
template <>
inline void SerializedObject::AddPrivateElementInArray<glm::vec2>(std::string const& arrayName, glm::vec2 const* element)
{
	SVec2 sVar = SVec2(*element);
	AddPrivateElementInArray(arrayName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPrivateElementInArray<glm::vec3>(std::string const& arrayName, glm::vec3 const* element)
{
	SVec3 sVar = SVec3(*element);
	AddPrivateElementInArray(arrayName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPrivateElementInArray<glm::vec4>(std::string const& arrayName, glm::vec4 const* element)
{
	SVec4 sVar = SVec4(*element);
	AddPrivateElementInArray(arrayName, static_cast<ISerializable const*>(&sVar));
}

/// ==========================================================================================

// GET PRIVATE ELEMENT
template <typename T>
inline void SerializedObject::GetPrivateElement(std::string const& elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PRIVATE_DATAS"][elementName];
}
template <>
inline void SerializedObject::GetPrivateElement<ISerializable>(std::string const& elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PRIVATE_DATAS"][elementName];
	outVariable = AutomaticRegisterISerializable<ISerializable>::create(jsonObject.GetType());
	outVariable->Deserialize(jsonObject);
}
template <>
inline void SerializedObject::GetPrivateElement<glm::vec2>(std::string const& elementName, glm::vec2* outVariable) const
{
	SVec2 sVar = SVec2(*outVariable);
	GetPrivateElement(elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y };
}
template <>
inline void SerializedObject::GetPrivateElement<glm::vec3>(std::string const& elementName, glm::vec3* outVariable) const
{
	SVec3 sVar = SVec3(*outVariable);
	GetPrivateElement(elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z };
}
template <>
inline void SerializedObject::GetPrivateElement<glm::vec4>(std::string const& elementName, glm::vec4* outVariable) const
{
	SVec4 sVar = SVec4(*outVariable);
	GetPrivateElement(elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z, sVar.w };
}


// GET PRIVATE Dictionary
template <typename T>
inline void SerializedObject::GetPrivateElementInDictionary(std::string const& dictionaryName, std::string const& elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PRIVATE_DATAS"][dictionaryName][elementName];
}
template <>
inline void SerializedObject::GetPrivateElementInDictionary<ISerializable>(std::string const& dictionaryName, std::string const& elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PRIVATE_DATAS"][dictionaryName][elementName];
	outVariable->Deserialize(jsonObject);
}
template <>
inline void SerializedObject::GetPrivateElementInDictionary<glm::vec2>(std::string const& dictionaryName, std::string const& elementName, glm::vec2* outVariable) const
{
	SVec2 sVar = SVec2(*outVariable);
	GetPrivateElementInDictionary(dictionaryName, elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y };
}
template <>
inline void SerializedObject::GetPrivateElementInDictionary<glm::vec3>(std::string const& dictionaryName, std::string const& elementName, glm::vec3* outVariable) const
{
	SVec3 sVar = SVec3(*outVariable);
	GetPrivateElementInDictionary(dictionaryName, elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z };
}
template <>
inline void SerializedObject::GetPrivateElementInDictionary<glm::vec4>(std::string const& dictionaryName, std::string const& elementName, glm::vec4* outVariable) const
{
	SVec4 sVar = SVec4(*outVariable);
	GetPrivateElementInDictionary(dictionaryName, elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z, sVar.w };
}

// GET PRIVATE ARRAY
template <typename T>
inline std::vector<T> SerializedObject::GetPrivateArray(std::string const& arrayName) const
{
	std::vector<T> array = {};

	for (uint32 i = 0; i < m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].size(); i++)
	{
		array.push_back(*m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName][i]);
	}

	return array;
}
template <>
inline std::vector<ISerializable*> SerializedObject::GetPrivateArray(std::string const& arrayName) const
{
	std::vector<ISerializable*> array = {};
	for (uint32 i = 0; i < m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].size(); i++)
	{
		SerializedObject jsonObject = {};
		jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName][i];
		ISerializable* outObject = AutomaticRegisterISerializable<ISerializable>::create(jsonObject.GetType());
		outObject->Deserialize(jsonObject);
		array.push_back(outObject);
	}

	return array;
}
template <>
inline std::vector<glm::vec2> SerializedObject::GetPrivateArray(std::string const& arrayName) const
{
	std::vector<glm::vec2> arrayOfElement = {};
	std::vector<ISerializable*> array = GetPrivateArray<ISerializable*>(arrayName);
	for (uint32 i = 0; i < array.size(); i++)
	{
		SVec2* vec = static_cast<SVec2*>(array[i]);
		arrayOfElement.push_back({ vec->x,vec->y });
	}
	return arrayOfElement;
}
template <>
inline std::vector<glm::vec3> SerializedObject::GetPrivateArray(std::string const& arrayName) const
{
	std::vector<glm::vec3> arrayOfElement = {};
	std::vector<ISerializable*> array = GetPrivateArray<ISerializable*>(arrayName);
	for (uint32 i = 0; i < array.size(); i++)
	{
		SVec3* vec = static_cast<SVec3*>(array[i]);
		arrayOfElement.push_back({ vec->x,vec->y,vec->z });
	}
	return arrayOfElement;
}
template <>
inline std::vector<glm::vec4> SerializedObject::GetPrivateArray(std::string const& arrayName) const
{
	std::vector<glm::vec4> arrayOfElement = {};
	std::vector<ISerializable*> array = GetPrivateArray<ISerializable*>(arrayName);
	for (uint32 i = 0; i < array.size(); i++)
	{
		SVec4* vec = static_cast<SVec4*>(array[i]);
		arrayOfElement.push_back({ vec->x,vec->y,vec->z,vec->w });
	}
	return arrayOfElement;
}

// =============== PUBLIC =================
// PUBLIC ELEMENT
template <typename T>
inline void SerializedObject::AddPublicElement(std::string const& variableName, T const* variableData)
{
	m_elementsInSerializedObject["PUBLIC_DATAS"][variableName] = *variableData;
}
template <>
inline void SerializedObject::AddPublicElement<ISerializable>(std::string const& variableName, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PUBLIC_DATAS"][variableName] = object.m_elementsInSerializedObject;
}
template <>
inline void SerializedObject::AddPublicElement<glm::vec2>(std::string const& variableName, glm::vec2 const* variableData)
{
	SVec2 sVar = SVec2(*variableData);
	AddPublicElement(variableName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPublicElement<glm::vec3>(std::string const& variableName, glm::vec3 const* variableData)
{
	SVec3 sVar = SVec3(*variableData);
	AddPublicElement(variableName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPublicElement<glm::vec4>(std::string const& variableName, glm::vec4 const* variableData)
{
	SVec4 sVar = SVec4(*variableData);
	AddPublicElement(variableName, static_cast<ISerializable const*>(&sVar));
}


// PUBLIC Dictionary
template <typename T>
inline void SerializedObject::AddPublicElementInDictionary(std::string const& disctionnaryName, std::string const& name, T const* element)
{
	m_elementsInSerializedObject["PUBLIC_DATAS"][disctionnaryName][name] = *element;
}
template <>
inline void SerializedObject::AddPublicElementInDictionary<ISerializable>(std::string const& disctionnaryName, std::string const& name, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PUBLIC_DATAS"][disctionnaryName][name] = object.m_elementsInSerializedObject;
}
template <>
inline void SerializedObject::AddPublicElementInDictionary<glm::vec2>(std::string const& disctionnaryName, std::string const& name, glm::vec2 const* element)
{
	SVec2 sVar = SVec2(*element);
	AddPublicElementInDictionary(disctionnaryName, name, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPublicElementInDictionary<glm::vec3>(std::string const& disctionnaryName, std::string const& name, glm::vec3 const* element)
{
	SVec3 sVar = SVec3(*element);
	AddPublicElementInDictionary(disctionnaryName, name, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPublicElementInDictionary<glm::vec4>(std::string const& disctionnaryName, std::string const& name, glm::vec4 const* element)
{
	SVec4 sVar = SVec4(*element);
	AddPublicElementInDictionary(disctionnaryName, name, static_cast<ISerializable const*>(&sVar));
}


// ADD PUBLIC ARRAY
template <typename T>
inline void SerializedObject::AddPublicElementInArray(std::string const& arrayName, T const* element)
{
	m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].push_back(element);
}
template <>
inline void SerializedObject::AddPublicElementInArray<ISerializable>(std::string const& arrayName, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].push_back(object.m_elementsInSerializedObject);
}
template <>
inline void SerializedObject::AddPublicElementInArray<glm::vec2>(std::string const& arrayName, glm::vec2 const* element)
{
	SVec2 sVar = SVec2(*element);
	AddPublicElementInArray(arrayName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPublicElementInArray<glm::vec3>(std::string const& arrayName, glm::vec3 const* element)
{
	SVec3 sVar = SVec3(*element);
	AddPublicElementInArray(arrayName, static_cast<ISerializable const*>(&sVar));
}
template <>
inline void SerializedObject::AddPublicElementInArray<glm::vec4>(std::string const& arrayName, glm::vec4 const* element)
{
	SVec4 sVar = SVec4(*element);
	AddPublicElementInArray(arrayName, static_cast<ISerializable const*>(&sVar));
}


/// ==========================================================================================

// GET PUBLIC ELEMENT
template <typename T>
inline void SerializedObject::GetPublicElement(std::string const& elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PUBLIC_DATAS"][elementName]/*["DATA"]*/;
}
template <>
inline void SerializedObject::GetPublicElement<ISerializable>(std::string const& elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PUBLIC_DATAS"][elementName];
	outVariable->Deserialize(jsonObject);
}
template <>
inline void SerializedObject::GetPublicElement<glm::vec2>(std::string const& elementName, glm::vec2* outVariable) const
{
	SVec2 sVar = SVec2(*outVariable);
	GetPublicElement(elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y };
}
template <>
inline void SerializedObject::GetPublicElement<glm::vec3>(std::string const& elementName, glm::vec3* outVariable) const
{
	SVec3 sVar = SVec3(*outVariable);
	GetPublicElement(elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z };
}
template <>
inline void SerializedObject::GetPublicElement<glm::vec4>(std::string const& elementName, glm::vec4* outVariable) const
{
	SVec4 sVar = SVec4(*outVariable);
	GetPublicElement(elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z, sVar.w };
}

// GET PUBLIC Dictionary
template <typename T>
inline void SerializedObject::GetPublicElementInDictionary(std::string const& dictionaryName, std::string const& elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PUBLIC_DATAS"][dictionaryName][elementName];
}
template <>
inline void SerializedObject::GetPublicElementInDictionary<ISerializable>(std::string const& dictionaryName, std::string const& elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PUBLIC_DATAS"][dictionaryName][elementName];
	outVariable->Deserialize(jsonObject);
}
template <>
inline void SerializedObject::GetPublicElementInDictionary<glm::vec2>(std::string const& dictionaryName, std::string const& elementName, glm::vec2* outVariable) const
{
	SVec2 sVar = SVec2(*outVariable);
	GetPublicElementInDictionary(dictionaryName, elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y };
}
template <>
inline void SerializedObject::GetPublicElementInDictionary<glm::vec3>(std::string const& dictionaryName, std::string const& elementName, glm::vec3* outVariable) const
{
	SVec3 sVar = SVec3(*outVariable);
	GetPublicElementInDictionary(dictionaryName, elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z };
}
template <>
inline void SerializedObject::GetPublicElementInDictionary<glm::vec4>(std::string const& dictionaryName, std::string const& elementName, glm::vec4* outVariable) const
{
	SVec4 sVar = SVec4(*outVariable);
	GetPublicElementInDictionary(dictionaryName, elementName, static_cast<ISerializable*>(&sVar));
	*outVariable = { sVar.x,sVar.y, sVar.z, sVar.w };
}


// GET PUBLIC ARRAY
template <typename T>
inline std::vector<T> SerializedObject::GetPublicArray(std::string const& arrayName) const
{
	std::vector<T> array = {};

	for (uint32 i = 0; i < m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].size(); i++)
	{
		array.push_back(m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName][i]);
	}

	return array;
}
template <>
inline std::vector<ISerializable*> SerializedObject::GetPublicArray(std::string const& arrayName) const
{
	std::vector<ISerializable*> array = {};
	for (uint32 i = 0; i < m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].size(); i++)
	{
		SerializedObject jsonObject = {};
		jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName][i];
		ISerializable* outObject = AutomaticRegisterISerializable<ISerializable>::create(jsonObject.GetType());
		outObject->Deserialize(jsonObject);
		array.push_back(outObject);
	}

	return array;
}
template <>
inline std::vector<glm::vec2> SerializedObject::GetPublicArray(std::string const& arrayName) const
{
	std::vector<glm::vec2> arrayOfElement = {};
	std::vector<ISerializable*> array = GetPublicArray<ISerializable*>(arrayName);
	for (uint32 i = 0; i < array.size(); i++)
	{
		SVec2* vec = static_cast<SVec2*>(array[i]);
		arrayOfElement.push_back({ vec->x,vec->y });
	}
	return arrayOfElement;
}
template <>
inline std::vector<glm::vec3> SerializedObject::GetPublicArray(std::string const& arrayName) const
{
	std::vector<glm::vec3> arrayOfElement = {};
	std::vector<ISerializable*> array = GetPublicArray<ISerializable*>(arrayName);
	for (uint32 i = 0; i < array.size(); i++)
	{
		SVec3* vec = static_cast<SVec3*>(array[i]);
		arrayOfElement.push_back({ vec->x,vec->y,vec->z });
	}
	return arrayOfElement;
}
template <>
inline std::vector<glm::vec4> SerializedObject::GetPublicArray(std::string const& arrayName) const
{
	std::vector<glm::vec4> arrayOfElement = {};
	std::vector<ISerializable*> array = GetPublicArray<ISerializable*>(arrayName);
	for (uint32 i = 0; i < array.size(); i++)
	{
		SVec4* vec = static_cast<SVec4*>(array[i]);
		arrayOfElement.push_back({ vec->x,vec->y,vec->z,vec->w });
	}
	return arrayOfElement;
}


#endif


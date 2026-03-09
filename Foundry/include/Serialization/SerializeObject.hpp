#ifndef _SERIALIZEOBJECT__H
#define _SERIALIZEOBJECT__H

#include "ISerializable.h"
#include "json.hpp"

#include <Define.h>
#include <Logger.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

/*
	* Object used to convert Data of an object in json
*/
class SerializedObject
{
public:
	SerializedObject();

	template<typename T>
	void SetType();

	std::string GetType() const;
	
	// PRIVATE
	void AddPrivateArray(std::string arrayName);
	void AddPrivateDictionnary(std::string dictionnaryName);

	template <typename T>
	void AddPrivateElement(std::string variableName, T const* variableData);

	template <typename T>
	void AddPrivateElementInDictionnary(std::string disctionnaryName, std::string name, T const* element);

	template <typename T>
	void AddPrivateElementInArray(std::string arrayName,T const* element);

	template <typename T>
	void GetPrivateElement(std::string elementName, T* outVariable) const;

	template <typename T>
	std::vector<T> GetPrivateArray(std::string arrayName) const;

	template <typename T>
	void GetPrivateElementInDictionnary(std::string dictionnaryName, std::string elementName, T* outVariable) const;

	// PUBLIC
	void AddPublicArray(std::string arrayName);
	void AddPublicDictionnary(std::string dictionnaryName);

	template <typename T>
	void AddPublicElement(std::string variableName, T const* variableData);

	template <typename T>
	void AddPublicElementInDictionnary(std::string disctionnaryName, std::string name, T const* element);

	template <typename T>
	void AddPublicElementInArray(std::string arrayName, T const* element);

	template <typename T>
	void GetPublicElement(std::string elementName, T* outVariable) const;

	template <typename T>
	std::vector<T> GetPublicArray(std::string arrayName) const;

	template <typename T>
	void GetPublicElementInDictionnary(std::string dictionnaryName, std::string elementName, T* outVariable) const;


	json const& GetJson() {return m_elementsInSerializedObject;}
	void SetJson(json const& json) { m_elementsInSerializedObject = json; } // Check d'error

private:
	std::vector<std::string> m_existingNames;
	json m_elementsInSerializedObject;
	friend class EditorSerializer;
};
template <typename T>
void SerializedObject::SetType()
{
	std::string t = typeid(T).name();
	m_elementsInSerializedObject["PRIVATE_DATAS"]["TYPE"] = t;
}


template <typename T>
inline void SerializedObject::AddPrivateElement(std::string variableName, T const* variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), variableName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject["PRIVATE_DATAS"][variableName] = *variableData;
	m_existingNames.push_back(variableName);
}

template <>
inline void SerializedObject::AddPrivateElement<ISerializable>(std::string variableName, ISerializable const* variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), variableName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}


	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PRIVATE_DATAS"][variableName] = object.m_elementsInSerializedObject;
	m_existingNames.push_back(variableName);
}


template <typename T>
inline void SerializedObject::AddPrivateElementInDictionnary(std::string disctionnaryName, std::string name, T const* element)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), disctionnaryName +"d"+name) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + name + " already store in Dictionnary " + disctionnaryName);
		return;
	}
	
	m_elementsInSerializedObject["PRIVATE_DATAS"][disctionnaryName][name] = *element;
	m_existingNames.push_back(disctionnaryName+"d"+name);
}
template <>
inline void SerializedObject::AddPrivateElementInDictionnary<ISerializable>(std::string disctionnaryName, std::string name, ISerializable const* variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), disctionnaryName + "d" + name) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + name + " already store in Dictionnary " + disctionnaryName);
		return;
	}
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PRIVATE_DATAS"][disctionnaryName][name] = object.m_elementsInSerializedObject;
	m_existingNames.push_back(disctionnaryName + "d" + name);
}

template <typename T>
inline void SerializedObject::AddPrivateElementInArray(std::string arrayName, T const* element)
{
	m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].push_back(*element);
}
template <>
inline void SerializedObject::AddPrivateElementInArray<ISerializable>(std::string arrayName, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].push_back(object.m_elementsInSerializedObject);
}

/// ==========================================================================================

template <typename T>
inline void SerializedObject::GetPrivateElement(std::string elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PRIVATE_DATAS"][elementName];
}
template <>
inline void SerializedObject::GetPrivateElement<ISerializable>(std::string elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PRIVATE_DATAS"][elementName];
	ISerializable* outObject = ISerializable::s_constructors[jsonObject.GetType()]();
	outObject->Deserialize(jsonObject);
	outVariable = outObject;
}


template <typename T>
inline void SerializedObject::GetPrivateElementInDictionnary(std::string dictionnaryName, std::string elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PRIVATE_DATAS"][dictionnaryName][elementName];
}

template <>
inline void SerializedObject::GetPrivateElementInDictionnary<ISerializable>(std::string dictionnaryName, std::string elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PRIVATE_DATAS"][dictionnaryName][elementName];
	ISerializable* outObject = ISerializable::s_constructors[jsonObject.GetType()]();
	outObject->Deserialize(jsonObject);
	outVariable = outObject;
}

template <typename T>
inline std::vector<T> SerializedObject::GetPrivateArray(std::string arrayName) const
{
	std::vector<T> array = {};

	for (uint32 i = 0; i < m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].size(); i++)
	{
		array.push_back(*m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName][i]);
	}

	return array;
}
template <>
inline std::vector<ISerializable*> SerializedObject::GetPrivateArray(std::string arrayName) const
{
	std::vector<ISerializable*> array = {};
	for (uint32 i = 0; i < m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName].size(); i++)
	{
		SerializedObject jsonObject = {};
		jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName][i];
		ISerializable* outObject = ISerializable::s_constructors[jsonObject.GetType()]();
		outObject->Deserialize(jsonObject);
		array.push_back(outObject);
	}

	return array;
}

// PUBLIC

template <typename T>
inline void SerializedObject::AddPublicElement(std::string variableName, T const* variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), variableName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}

	m_elementsInSerializedObject["PUBLIC_DATAS"][variableName] = *variableData;
	m_existingNames.push_back(variableName);
}

template <>
inline void SerializedObject::AddPublicElement<ISerializable>(std::string variableName, ISerializable const* variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), variableName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}

	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PUBLIC_DATAS"][variableName] = object.m_elementsInSerializedObject;
	m_existingNames.push_back(variableName);
}


template <typename T>
inline void SerializedObject::AddPublicElementInDictionnary(std::string disctionnaryName, std::string name, T const* element)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), disctionnaryName +"d"+name) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + name + " already store in Dictionnary " + disctionnaryName);
		return;
	}
	
	m_elementsInSerializedObject["PUBLIC_DATAS"][disctionnaryName][name] = *element;
	m_existingNames.push_back(disctionnaryName+"d"+name);
}
template <>
inline void SerializedObject::AddPublicElementInDictionnary<ISerializable>(std::string disctionnaryName, std::string name, ISerializable const* variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), disctionnaryName + "d" + name) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + name + " already store in Dictionnary " + disctionnaryName);
		return;
	}

	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PUBLIC_DATAS"][disctionnaryName][name] = object.m_elementsInSerializedObject;
	m_existingNames.push_back(disctionnaryName + "d" + name);
}

template <typename T>
inline void SerializedObject::AddPublicElementInArray(std::string arrayName, T const* element)
{
	m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].push_back(element);
}
template <>
inline void SerializedObject::AddPublicElementInArray<ISerializable>(std::string arrayName, ISerializable const* variableData)
{
	SerializedObject object = SerializedObject();
	if (variableData != nullptr)
		variableData->Serialize(object);

	m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].push_back(object.m_elementsInSerializedObject);
}

/// ==========================================================================================

template <typename T>
inline void SerializedObject::GetPublicElement(std::string elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PUBLIC_DATAS"][elementName];
}
template <>
inline void SerializedObject::GetPublicElement<ISerializable>(std::string elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PUBLIC_DATAS"][elementName];
	ISerializable* outObject = ISerializable::s_constructors[jsonObject.GetType()]();
	outObject->Deserialize(jsonObject);
	outVariable = outObject;
}

template <typename T>
inline void SerializedObject::GetPublicElementInDictionnary(std::string dictionnaryName, std::string elementName, T* outVariable) const
{
	*outVariable = m_elementsInSerializedObject["PUBLIC_DATAS"][dictionnaryName][elementName];
}

template <>
inline void SerializedObject::GetPublicElementInDictionnary<ISerializable>(std::string dictionnaryName, std::string elementName, ISerializable* outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PUBLIC_DATAS"][dictionnaryName][elementName];
	ISerializable* outObject = ISerializable::s_constructors[jsonObject.GetType()]();
	outObject->Deserialize(jsonObject);
	outVariable = outObject;
}

template <typename T>
inline std::vector<T> SerializedObject::GetPublicArray(std::string arrayName) const
{
	std::vector<T> array = {};

	for (uint32 i = 0; i < m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].size(); i++)
	{
		array.push_back(m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName][i]);
	}

	return array;
}
template <>
inline std::vector<ISerializable*> SerializedObject::GetPublicArray(std::string arrayName) const
{
	std::vector<ISerializable*> array = {};
	for (uint32 i = 0; i < m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName].size(); i++)
	{
		SerializedObject jsonObject = {};
		jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName][i];
		ISerializable* outObject = ISerializable::s_constructors[jsonObject.GetType()]();
		outObject->Deserialize(jsonObject);
		array.push_back(outObject);
	}

	return array;
}


#endif


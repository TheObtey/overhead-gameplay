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

	template<typename T>
	void SetType();
	
	void AddArray(std::string arrayName);
	void AddDictionnary(std::string dictionnaryName);

	template <typename T>
	void AddElement(std::string variableName, T const& variableData);

	template <typename T>
	void AddElementInDictionnary(std::string disctionnaryName, std::string name, T const& element);

	template <typename T>
	void AddElementInArray(std::string arrayName,T const& element);

	void GetType(std::string& type) const;
	
	template <typename T>
	void GetElement(std::string elementName, T& outVariable) const;

	template <typename T>
	std::vector<T> GetArray(std::string arrayName) const;

	template <typename T>
	void GetElementInDictionnary(std::string dictionnaryName, std::string elementName, T& outVariable) const;

private:
	std::vector<std::string> m_existingNames;
	json m_elementsInSerializedObject;
	friend class EditorSerializer;
};
template <typename T>
void SerializedObject::SetType()
{
	std::string t = typeid(T).name();
	m_elementsInSerializedObject["TYPE"] = t;
}


template <typename T>
inline void SerializedObject::AddElement(std::string variableName, T const& variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), variableName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject[variableName] = variableData;
	m_existingNames.push_back(variableName);
}

template <>
inline void SerializedObject::AddElement<ISerializable>(std::string variableName, ISerializable const& variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), variableName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}

	SerializedObject object = {};
	variableData.Serialize(object);

	m_elementsInSerializedObject[variableName] = object.m_elementsInSerializedObject;
	m_existingNames.push_back(variableName);
}


template <typename T>
inline void SerializedObject::AddElementInDictionnary(std::string disctionnaryName, std::string name, T const& element)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), disctionnaryName +"d"+name) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + name + " already store in Dictionnary " + disctionnaryName);
		return;
	}
	
	m_elementsInSerializedObject[disctionnaryName][name] = element;
	m_existingNames.push_back(disctionnaryName+"d"+name);
}
template <>
inline void SerializedObject::AddElementInDictionnary<ISerializable>(std::string disctionnaryName, std::string name, ISerializable const& variableData)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), disctionnaryName + "d" + name) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + name + " already store in Dictionnary " + disctionnaryName);
		return;
	}
	SerializedObject object = {};
	variableData.Serialize(object);
	m_elementsInSerializedObject[disctionnaryName][name] = object.m_elementsInSerializedObject;
	m_existingNames.push_back(disctionnaryName + "d" + name);
}

template <typename T>
inline void SerializedObject::AddElementInArray(std::string arrayName, T const& element)
{
	m_elementsInSerializedObject[arrayName].push_back(element);
}
template <>
inline void SerializedObject::AddElementInArray<ISerializable>(std::string arrayName, ISerializable const& variableData)
{
	SerializedObject object = {};
	variableData.Serialize(object);
	m_elementsInSerializedObject[arrayName].push_back(object.m_elementsInSerializedObject);
}

/// ==========================================================================================

template <typename T>
inline void SerializedObject::GetElement(std::string elementName, T& outVariable) const
{
	outVariable = m_elementsInSerializedObject[elementName];
}
template <>
inline void SerializedObject::GetElement<ISerializable>(std::string elementName, ISerializable& outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject[elementName];
	std::string type;
	jsonObject.GetType(type);
	ISerializable* outObject = ISerializable::s_constructors[type]();
	outObject->Deserialize(jsonObject);
	outVariable = *outObject;
}


template <typename T>
inline void SerializedObject::GetElementInDictionnary(std::string dictionnaryName, std::string elementName, T& outVariable) const
{
	outVariable = m_elementsInSerializedObject[dictionnaryName][elementName];
}

template <>
inline void SerializedObject::GetElementInDictionnary<ISerializable>(std::string dictionnaryName, std::string elementName, ISerializable& outVariable) const
{
	SerializedObject jsonObject = {};
	jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject[dictionnaryName][elementName];
	std::string type;
	jsonObject.GetType(type);
	ISerializable* outObject = ISerializable::s_constructors[type]();
	outObject->Deserialize(jsonObject);
	outVariable = *outObject;
}

template <typename T>
inline std::vector<T> SerializedObject::GetArray(std::string arrayName) const
{
	std::vector<T> array = {};

	for (uint32 i = 0; i < m_elementsInSerializedObject[arrayName].size(); i++)
	{
		array.push_back(m_elementsInSerializedObject[arrayName][i]);
	}

	return array;
}
template <>
inline std::vector<ISerializable*> SerializedObject::GetArray(std::string arrayName) const
{
	std::vector<ISerializable*> array = {};
	for (uint32 i = 0; i < m_elementsInSerializedObject[arrayName].size(); i++)
	{
		SerializedObject jsonObject = {};
		jsonObject.m_elementsInSerializedObject = m_elementsInSerializedObject[arrayName][i];
		std::string type;
		jsonObject.GetType(type);
		ISerializable* outObject = ISerializable::s_constructors[type]();
		outObject->Deserialize(jsonObject);
		array.push_back(outObject);
	}

	return array;
}


#endif


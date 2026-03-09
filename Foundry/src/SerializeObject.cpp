#include "Serialization/SerializeObject.hpp"

SerializedObject::SerializedObject()
{
	m_elementsInSerializedObject = json::object();
	m_elementsInSerializedObject["PRIVATE_DATAS"] = json::object();
	m_elementsInSerializedObject["PUBLIC_DATAS"] = json::object();
}

std::string SerializedObject::GetType() const
{
	return m_elementsInSerializedObject["PRIVATE_DATAS"]["TYPE"];
}

void SerializedObject::AddPrivateArray(std::string arrayName)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), arrayName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + arrayName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject["PRIVATE_DATAS"][arrayName] = json::array();
	m_existingNames.push_back(arrayName);
}

void SerializedObject::AddPrivateDictionnary(std::string dictionnaryName)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), dictionnaryName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + dictionnaryName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject["PRIVATE_DATAS"][dictionnaryName] = json::object();
	m_existingNames.push_back(dictionnaryName);
}

void SerializedObject::AddPublicArray(std::string arrayName)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), arrayName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + arrayName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject["PUBLIC_DATAS"][arrayName] = json::array();
	m_existingNames.push_back(arrayName);
}
void SerializedObject::AddPublicDictionnary(std::string dictionnaryName)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), dictionnaryName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + dictionnaryName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject["PUBLIC_DATAS"][dictionnaryName] = json::object();
	m_existingNames.push_back(dictionnaryName);
}
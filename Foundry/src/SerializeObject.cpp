#include "Serialization/SerializeObject.hpp"

void SerializedObject::GetType(std::string& type) const
{
	type = m_elementsInSerializedObject["TYPE"];
}

void SerializedObject::AddArray(std::string arrayName)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), arrayName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + arrayName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject[arrayName] = json::array();
	m_existingNames.push_back(arrayName);
}
void SerializedObject::AddDictionnary(std::string dictionnaryName)
{
	if (std::find(m_existingNames.begin(), m_existingNames.end(), dictionnaryName) != m_existingNames.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + dictionnaryName + " already store in Data");
		return;
	}
	m_elementsInSerializedObject[dictionnaryName] = json::object();
	m_existingNames.push_back(dictionnaryName);
}
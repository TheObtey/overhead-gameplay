#include "ISerializable.h"

#include <Logger.hpp>


void SerializeData::AddVariable(std::string variableName, std::string const& variableData)
{
	if (m_datas.find(variableName) != m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_datas.insert({ variableName,variableData });
}
void SerializeData::AddVariable(std::string variableName, int32 const& variableData)
{
	if (m_datas.find(variableName) != m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR,"Variable " + variableName + " already store in Data");
		return;
	}
	m_datas.insert({ variableName, std::to_string(variableData) });
}
void SerializeData::AddVariable(std::string variableName, float const& variableData)
{
	if (m_datas.find(variableName) != m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_datas.insert({ variableName,std::to_string(variableData) });
}
void SerializeData::AddVariable(std::string variableName, bool const& variableData)
{
	if (m_datas.find(variableName) != m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_datas.insert({ variableName, variableData ? "true" : "false" });
}
void SerializeData::AddVariable(std::string variableName, glm::vec2 const& variableData)
{
	if (m_datas.find(variableName) != m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_datas.insert({ variableName,ConvertVarToString(variableData) });
}
void SerializeData::AddVariable(std::string variableName, glm::vec3 const& variableData)
{
	if (m_datas.find(variableName) != m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_datas.insert({ variableName,ConvertVarToString(variableData) });
}
void SerializeData::AddVariable(std::string variableName, glm::vec4 const& variableData)
{
	if (m_datas.find(variableName) != m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " already store in Data");
		return;
	}
	m_datas.insert({ variableName,ConvertVarToString(variableData) });
}

std::string SerializeData::ConvertVarToString(glm::vec2 variableData)
{
	std::string variable = "";
	variable += std::to_string(variableData.x) + ",";
	variable += std::to_string(variableData.y);
	return variable;
}
std::string SerializeData::ConvertVarToString(glm::vec3 variableData)
{
	std::string variable = "";
	variable += std::to_string(variableData.x) + ",";
	variable += std::to_string(variableData.y) + ",";
	variable += std::to_string(variableData.z);
	return variable;
}
std::string SerializeData::ConvertVarToString(glm::vec4 variableData)
{
	std::string variable = "";
	variable += std::to_string(variableData.x) + ",";
	variable += std::to_string(variableData.y) + ",";
	variable += std::to_string(variableData.z) + ",";
	variable += std::to_string(variableData.w);
	return variable;
}


void SerializeData::GetVariable(std::string variableName, std::string& variable) const
{
	if (m_datas.find(variableName) == m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " not stored in Datas");
		return;
	}
	variable = m_datas.at(variableName);
}
void SerializeData::GetVariable(std::string variableName, int32& variableData) const
{
	if (m_datas.find(variableName) == m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " not stored in Datas");
		return;
	}
	variableData = std::stoi(m_datas.at(variableName));
}
void SerializeData::GetVariable(std::string variableName, float& variableData) const
{
	if (m_datas.find(variableName) == m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " not stored in Datas");
		return;
	}
	variableData = std::stof(m_datas.at(variableName));
}
void SerializeData::GetVariable(std::string variableName, bool& variableData) const
{
	if (m_datas.find(variableName) == m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " not stored in Datas");
		return;
	}
	variableData = m_datas.at(variableName) == "true" ? true : false;
}
void SerializeData::GetVariable(std::string variableName, glm::vec2& variableData) const
{
	if (m_datas.find(variableName) == m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " not stored in Datas");
		return;
	}
	variableData = ConvertStringToVec2(variableName);
}
void SerializeData::GetVariable(std::string variableName, glm::vec3& variableData) const
{
	if (m_datas.find(variableName) == m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " not stored in Datas");
		return;
	}
	variableData = ConvertStringToVec3(variableName);
}
void SerializeData::GetVariable(std::string variableName, glm::vec4& variableData) const
{
	if (m_datas.find(variableName) == m_datas.end())
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Variable " + variableName + " not stored in Datas");
		return;
	}
	variableData = ConvertStringToVec4(variableName);
}

glm::vec2 SerializeData::ConvertStringToVec2(std::string variableName) const
{
	glm::vec2 result = {};
	std::string sVec3 = m_datas.at(variableName);

	int32 it = sVec3.find(",");
	result.x = std::stof(sVec3.substr(0, it));
	sVec3.erase(sVec3.begin(), sVec3.begin() + it+1);

	it = sVec3.find(",");
	result.y = std::stof(sVec3.substr(0, it));

	return result;
}
glm::vec3 SerializeData::ConvertStringToVec3(std::string variableName) const
{
	glm::vec3 result = {};
	std::string sVec3 = m_datas.at(variableName);

	int32 it = sVec3.find(",");
	result.x = std::stof(sVec3.substr(0, it));
	sVec3.erase(sVec3.begin(), sVec3.begin()+it+1);

	it = sVec3.find(",");
	result.y = std::stof(sVec3.substr(0, it));
	sVec3.erase(sVec3.begin(), sVec3.begin() + it+1);

	it = sVec3.find(",");
	result.z = std::stof(sVec3.substr(0, it));
	return result;
}
glm::vec4 SerializeData::ConvertStringToVec4(std::string variableName) const
{
	glm::vec4 result = {};
	std::string sVec4 = m_datas.at(variableName);

	int32 it = sVec4.find(",");
	result.x = std::stof(sVec4.substr(0, it));
	sVec4.erase(sVec4.begin(), sVec4.begin() + it + 1);

	it = sVec4.find(",");
	result.y = std::stof(sVec4.substr(0, it));
	sVec4.erase(sVec4.begin(), sVec4.begin() + it + 1);

	it = sVec4.find(",");
	result.z = std::stof(sVec4.substr(0, it));
	sVec4.erase(sVec4.begin(), sVec4.begin() + it + 1);

	it = sVec4.find(",");
	result.w = std::stof(sVec4.substr(0, it));
	return result;
}


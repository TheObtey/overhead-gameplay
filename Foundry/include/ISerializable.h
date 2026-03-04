#ifndef _ISERIALIZABLE__H
#define _ISERIALIZABLE__H

#include <map>
#include <string>
#include <Define.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class SerializeObject;

/*
	* Used in Node to convert a variable in a string and store it in a std::map<sdt::string,std::string>
*/

class SerializeData
{
public:
	void AddVariable(std::string variableName, std::string const& variableData);
	void AddVariable(std::string variableName, int32 const& variableData);
	void AddVariable(std::string variableName, float const& variableData);
	void AddVariable(std::string variableName, bool const& variableData);
	void AddVariable(std::string variableName, glm::vec2 const& variableData);
	void AddVariable(std::string variableName, glm::vec3 const& variableData);
	void AddVariable(std::string variableName, glm::vec4 const& variableData);

	void GetVariable(std::string variableName, std::string& variable) const;
	void GetVariable(std::string variableName, int32& variableData) const;
	void GetVariable(std::string variableName, float& variableData) const;
	void GetVariable(std::string variableName, bool& variableData) const;
	void GetVariable(std::string variableName, glm::vec2& variableData) const;
	void GetVariable(std::string variableName, glm::vec3& variableData) const;
	void GetVariable(std::string variableName, glm::vec4& variableData) const;

private:
	std::string ConvertVarToString(glm::vec2 variableData);
	std::string ConvertVarToString(glm::vec3 variableData);
	std::string ConvertVarToString(glm::vec4 variableData);

	glm::vec2 ConvertStringToVec2(std::string variableName) const;
	glm::vec3 ConvertStringToVec3(std::string variableName) const;
	glm::vec4 ConvertStringToVec4(std::string variableName) const;

private:
	std::map<std::string, std::string> m_datas;

	friend SerializeObject;
};

/*
	All Objects used in Editor must inherit from ISerializable to be used in Editor
*/
class ISerializable
{
public:
	virtual void Serialize(SerializeData& datas) = 0;
	virtual void Deserialize(SerializeData const& datas) = 0;
};

#endif
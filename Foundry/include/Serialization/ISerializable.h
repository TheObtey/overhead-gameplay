#ifndef _ISERIALIZABLE__H
#define _ISERIALIZABLE__H

#include "json.hpp"

#include <string>
#include <unordered_map>

class SerializedObject;

/*
	* All Objects used in Editor must inherit from ISerializable to be used in Editor
	* Store constructor of ISerializable for AutomaticRegistration
*/
class ISerializable
{
public:
	virtual void Serialize(SerializedObject& datas) const = 0;
	virtual void Deserialize(SerializedObject const& datas) = 0;

	static inline std::unordered_map<std::string, std::function<ISerializable*()>> s_constructors{};
};
#endif
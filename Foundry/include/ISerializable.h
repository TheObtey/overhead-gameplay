#ifndef _ISERIALIZABLE__H
#define _ISERIALIZABLE__H

#include <map>
#include <string>

class SerializeObject;

/*
	All Objects used in Editor must inherit from ISerializable to be used in Editor
*/
class ISerializable
{
public:
	virtual std::map<std::string, std::string> const& Serialize() = 0;
	virtual void Deserialize(std::map<std::string, std::string> const& object) = 0;
};

#endif
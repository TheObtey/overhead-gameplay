
#include "EditorSerializer.h"

#include <Serialization/json.hpp>
#include <Serialization/SerializeObject.hpp>
#include <Servers/EngineServer.h>
#include <fstream>

using json = nlohmann::json;

void EditorSerializer::SaveScene(std::string outPath, uptr<Node>& root)
{
	EngineServer::FlushCommands();

	json jsonRoot = json::array();
	SerializedObject object;
	object.SetType("Node");
	root.get()->Serialize(object);
	jsonRoot[0]["Root"] = object.m_elementsInSerializedObject;

	std::fstream File;
	File.open(outPath + ".json", std::ios::out);
	File << jsonRoot;
	File.close();
}

void EditorSerializer::SaveNode(std::string outPath, uptr<Node>& node)
{
	EngineServer::FlushCommands();

	json jsonRoot = json::array();
	SerializedObject object;
	object.SetType("Node");
	node.get()->Serialize(object);
	jsonRoot[0]["Node"] = object.m_elementsInSerializedObject;

	std::fstream File;
	File.open(outPath + ".json", std::ios::out);
	File << jsonRoot;
	File.close();
}

LoadReturn EditorSerializer::LoadFromJson(std::string path)
{
	std::fstream file;
	file.open(path, std::ios::in);
	json jsonFile{ json::parse(file) };
	file.close();

	uptr<Node> firstNode = Node::CreateNode<Node>("Node");

	LoadReturn tmp;
	SerializedObject object = {};
	if (jsonFile[0].contains("Root")) {
		object.m_elementsInSerializedObject = jsonFile[0]["Root"];
		firstNode.get()->Deserialize(object);
		EngineServer::FlushCommands();
		tmp.IsRoot = true;
	}
	else if (jsonFile[0].contains("Node")) {
		object.m_elementsInSerializedObject = jsonFile[0]["Node"];
		firstNode.get()->Deserialize(object);
		EngineServer::FlushCommands();
		tmp.IsRoot = false;
	}
	tmp.uptrNode = std::move(firstNode);
	
	return tmp;
}

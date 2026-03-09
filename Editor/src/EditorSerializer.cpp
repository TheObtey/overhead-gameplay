
#include "EditorSerializer.h"

#include <Serialization/json.hpp>
#include <Serialization/SerializeObject.hpp>
#include <Servers/EngineServer.h>
#include <fstream>

using json = nlohmann::json;

void EditorSerializer::Save(std::string outPath, uptr<Node>& root)
{
	EngineServer::FlushCommands();

	json jsonRoot;
	SerializedObject object;
	object.SetType<Node>();
	root.get()->Serialize(object);
	jsonRoot["Root"] = object.m_elementsInSerializedObject;

	std::fstream File;
	File.open(outPath + ".json", std::ios::out);
	File << jsonRoot;
	File.close();
}

uptr<Node> EditorSerializer::LoadFromJson(std::string path)
{
	std::fstream file;
	file.open(path + ".json", std::ios::in);
	json jsonFile{ json::parse(file) };
	file.close();

	SerializedObject object = {};
	object.m_elementsInSerializedObject = jsonFile["Root"];
	uptr<Node> firstNode = Node::CreateNode<Node>("Node");
	firstNode.get()->Deserialize(object);

	EngineServer::FlushCommands();
	return std::move(firstNode);
}

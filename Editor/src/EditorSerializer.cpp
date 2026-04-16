#include "EditorSerializer.h"

#include <Serialization/json.hpp>
#include <Serialization/SerializeObject.hpp>
#include <Serialization/ISerializable.h>
#include <Registries/AutomaticRegister.hpp>
#include <Servers/EngineServer.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace
{
	uptr<Node> CreateNodeFromSerializedObject(SerializedObject const& object)
	{
		std::string const type = object.GetType();
		ISerializable* raw = AutomaticRegisterISerializable<ISerializable>::create(type);

		if (raw == nullptr)
		{
			return Node::CreateNode<Node>("Node");
		}

		Node* asNode = dynamic_cast<Node*>(raw);
		if (asNode == nullptr)
		{
			delete raw;
			std::cerr << "[EditorSerializer] Serialized type '" << type
				<< "' is not a Node, fallback to Node" << std::endl;
			return Node::CreateNode<Node>("Node");
		}

		return uptr<Node>(asNode);
	}
}

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
	json jsonFile(json::parse(file));
	file.close();

	LoadReturn tmp{};
	SerializedObject object{};

	if (jsonFile[0].contains("Root"))
	{
		object.SetJson(jsonFile[0]["Root"]);
		tmp.IsRoot = true;
	}
	else if (jsonFile[0].contains("Node"))
	{
		object.SetJson(jsonFile[0]["Node"]);
		tmp.IsRoot = false;
	}
	else
	{
		std::cerr << "[EditorSerializer] Invalid JSON file: missing Root/Node" << std::endl;
		tmp.uptrNode = Node::CreateNode<Node>("Node");
		return tmp;
	}

	uptr<Node> firstNode = CreateNodeFromSerializedObject(object);
	firstNode->Deserialize(object);

	EngineServer::FlushCommands();
	tmp.uptrNode = std::move(firstNode);
	return tmp;
}

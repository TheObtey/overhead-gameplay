
#include "SerializeObject.h"

#include <fstream>


void SerializeObject::Save(std::string outPath, uptr<Node>& root)
{
	json jsonRoot;
	jsonRoot["Root"]["TYPE"] = 0;
	jsonRoot["Root"]["Datas"] = root.get()->Serialize();
	for (uint8 i = 0; i < root.get()->GetChildCount();i++)
	{
		jsonRoot["Root"]["Children"][i] = ParseNodeToJson(root.get()->GetChild(i), jsonRoot["Root"]["Children"][i]);
	}

	std::fstream File;
	File.open(outPath, std::ios::out);
	File << jsonRoot;
	File.close();
}

json SerializeObject::ParseNodeToJson(Node& pNode, json jsonData)
{
	jsonData["TYPE"] = GetTypeFromNode(&pNode);
	jsonData["Datas"] = pNode.Serialize();
	for (uint8 i = 0; i < pNode.GetChildCount(); i++)
	{
		jsonData["Children"][i] = ParseNodeToJson(pNode.GetChild(i), jsonData["Children"][i]);
	}

	return jsonData;
}

uptr<Node> SerializeObject::LoadFromJson(std::string path)
{
	//TODO replace with SceneTree
	// Read File nlhoman aled

	std::fstream file;
	file.open(path, std::ios::out);
	json jsonFile{ json::parse(file) };

	uptr<Node> firstNode = Node::CreateNode<Node>("Node");
	std::map<std::string, std::string> tempMap;
	jsonFile["Root"]["Datas"].get_to(tempMap);
	firstNode.get()->Deserialize(tempMap);
	json childrenList = jsonFile["Root"]["Children"];
	for (uint8 i = 0; i < childrenList.size(); i++)
	{
		firstNode.get()->AddChild(ParseNodeData(childrenList[i]));
	}

	file.close();

	return std::move(firstNode);
}

uptr<Node> SerializeObject::ParseNodeData(json data)
{
	uptr<Node> newNode = CreateNodeFromType(data["Type"]);
	std::map<std::string, std::string> tempMap;
	data["Datas"].get_to(tempMap);
	newNode.get()->Deserialize(tempMap);
	json childrenList = data["Children"];
	for (uint8 i = 0; i < childrenList.size(); i++)
	{
		newNode.get()->AddChild(ParseNodeData(childrenList[i]));
	}
	return std::move(newNode);
}

uptr<Node> SerializeObject::CreateNodeFromType(Type nodeType)
{
	
	switch (nodeType)
	{
	case Type::NODE:
		return std::move(Node::CreateNode<Node>("Node"));
		break;
	default:
		break;
	}

	// TODO: insérer une instruction return ici
}

Type SerializeObject::GetTypeFromNode(Node* pNode)
{
	if (dynamic_cast<Node*>(pNode))
		return Type::NODE;
}


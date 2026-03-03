
#include "SerializeObject.h"

#include <fstream>
#include <map>


void SerializeObject::Save(std::string outPath, uptr<Node>& root)
{
	json jsonRoot;
	jsonRoot["Root"] = json::object();
	std::map<std::string, std::string> mapDatas = root.get()->Serialize();
	jsonRoot["Root"]["Children"] = json::array();
	for (uint8 i = 0; i < root.get()->GetChildCount();i++)
	{
		jsonRoot["Root"]["Children"][i] = ParseNodeToJson(root.get()->GetChild(i));
	}
	jsonRoot["Root"]["Datas"] = mapDatas;
	jsonRoot["Root"]["TYPE"] = 0;

	std::fstream File;
	File.open("../res/" + outPath, std::ios::out);
	File << jsonRoot;
	File.close();
}

json SerializeObject::ParseNodeToJson(Node& pNode)
{
	json objectChildren = json::object();
	objectChildren["TYPE"] = GetTypeFromNode(&pNode);
	std::map<std::string, std::string> mapDatas = pNode.Serialize();
	objectChildren["Datas"] = mapDatas;
	objectChildren["Children"] = json::array();
	for (uint8 i = 0; i < pNode.GetChildCount(); i++)
	{
		objectChildren["Children"][i] = ParseNodeToJson(pNode.GetChild(i));
	}

	return objectChildren;
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


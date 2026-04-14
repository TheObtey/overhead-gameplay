#ifndef NODE__H_
#define NODE__H_

#include "Define.h"
#include "Event.hpp"
#include "Serialization/json.hpp"
#include "Serialization/ISerializable.h"
#include "Serialization/SerializeObject.hpp"
#include "Scripting/Lua/LuaScriptInstance.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <optional>
#include <string>
#include <sstream>
#include <vector>

class SerializedObject;
class SceneTree;

template <typename T>
concept NodeType = std::is_base_of_v<Node, T>;

//Maybe move this in a global namespace
template <typename T>
using OptionalRef = std::optional<std::reference_wrapper<T>>;

//Base class off every node in the tree
class Node : public ISerializable
{
public:
	class Proxy;

	virtual ~Node();

	void Update(double delta);
	virtual void OnUpdate(double delta) {  };
	void PhysicsUpdate(double delta);
	virtual void OnPhysicsUpdate(double delta) {  };

	void AddChild(std::unique_ptr<Node>&& child);
	void AddChild(std::unique_ptr<Node>& child);
	void AddSibling(std::unique_ptr<Node>&& sibling);
	void AddSibling(std::unique_ptr<Node>& sibling);

	void RemoveChild(Node& child);
	void RemoveChild(std::string const& childName);

	template <NodeType T>
	OptionalRef<T> FindChild(); //recursive children search
	OptionalRef<Node> FindChild(std::string const& name); //recursive children search

	Node& GetChild(uint32 index);
	std::vector<std::reference_wrapper<Node>> GetChildren();
	uint32 GetChildCount() const;

	template <NodeType T>
	OptionalRef<T> GetNode(std::string const& path);

	template <NodeType T>
	OptionalRef<T> FindFirstParentOfType();

	//Only destroy the node if it has a parent
	void Destroy();
	virtual void Reparent(Node& newParent, bool keepGlobalTransform = true);
	void MoveChild(Node const& child, uint32 to);
	uptr<Node> DetachFromTree();

	//override this method if the inherited node is not trivially copyable
	virtual std::unique_ptr<Node> Clone();
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;

	std::string GetName();
	void SetName(std::string const& name);
	void SetScriptPath(std::string const& path);
	Node* GetParent() const;
	bool HasParent() const;
	SceneTree* GetSceneTree() const;

	Proxy& GetNodeProxy() const;

	//====Static Methods======
	template <NodeType T>
	static std::unique_ptr<T> CreateNode(std::string const& name);

	template <NodeType T>
	static uptr<T> LoadNodeFromJSON(std::filesystem::path path);

	template <NodeType T>
	static void AttachScript(uptr<LuaScriptInstance>& script, T& node);

	static void SetStatusEditor(bool inEditor) { s_IsInEditor = inEditor; }
	static ISerializable* CreateInstance();
	static std::string BuildDuplicateName(Node& parent, std::string const& sourceName);

	//====Event======
	Event<void(Node&)> OnSceneEnter;
	Event<void(Node&, double)> OnNodeUpdated;
	Event<void(Node&, double)> OnNodePhysicsUpdated;
	Event<void(Node&)> OnSceneLeave;
	Event<void(Node&)> OnParentChange;
	Event<void()> OnHierarchyChanged;

protected:
	//private constructor for in-class initialization
	//====Constructors======
	Node() = delete;
	Node(std::string const& name);
	Node(Node const& other) = delete;
	Node(Node&& other) noexcept = delete;

	Node& operator=(Node const& other) = delete;
	Node& operator=(Node&& other) noexcept = delete;

	virtual void AttachScriptDeserialize(uptr<LuaScriptInstance>& script);

	std::string m_name; //unique among siblings
	std::string m_scriptPath;
	Node* m_pOwner = nullptr;
	SceneTree* m_pSceneTree = nullptr;

	uptr<Proxy> m_pProxy;
	uptr<LuaScriptInstance> m_pScriptInstance;

	std::unordered_map<std::string, std::unique_ptr<Node>> m_children{};
	std::vector<std::string> m_childrenOrder{};
	inline static bool s_IsInEditor = false;

private:
    void AttachChildImmediate(std::unique_ptr<Node>& child);
	void NotifyHierarchyChanged();
	Node* ParseFirstNode(std::string const& path);

    friend class EngineServer;
	friend class SceneTree;
	friend class unique_ptr;
};

REGISTER_ISERIALIZABLE(Node, Node::CreateInstance);

//=== Templated function def ====
template <NodeType T>
std::unique_ptr<T> Node::CreateNode(std::string const& name)
{
    struct concrete_Node: public T {
        concrete_Node(std::string const& name) : T(name) {}
    };

	uptr<concrete_Node> ptr = std::make_unique<concrete_Node>(name);
	ptr->m_pProxy = std::make_unique<typename T::Proxy>(*ptr);
    return std::move(ptr);
}

template <NodeType T>
uptr<T> Node::LoadNodeFromJSON(std::filesystem::path path)
{
	std::fstream file;
	file.open(path, std::ios::in);
	nlohmann::json jsonFile(nlohmann::json::parse(file));
	file.close();

	uptr<T> firstNode = Node::CreateNode<T>("Node");

	SerializedObject object = {};
	if (jsonFile[0].contains("Root")) {
		object.SetJson(jsonFile[0]["Root"]);
		firstNode.get()->Deserialize(object);
	}
	else if (jsonFile[0].contains("Node")) {
		object.SetJson(jsonFile[0]["Node"]);
		firstNode.get()->Deserialize(object);
	}
	return firstNode;
}


template <NodeType T>
void Node::AttachScript(uptr<LuaScriptInstance>& script, T& node)
{
	node.m_pScriptInstance = std::move(script);
	node.m_pScriptInstance->template AttachToProxy<typename T::Proxy>(static_cast<T::Proxy*>(node.m_pProxy.get()));
}

template <NodeType T>
OptionalRef<T> Node::FindChild()
{
	for (auto&[nodeName, nodePtr] : m_children)
	{
		if (dynamic_cast<T*>(nodePtr.get()))
			return {*nodePtr.get()};
	}
	return {};
}

template <NodeType T>
OptionalRef<T> Node::GetNode(std::string const& path)
{
    std::stringstream ss(path);
    std::string token;

    //TODO Add this when scene tree is done
	Node* pNode = ParseFirstNode(path);

    while(std::getline(ss, token, '/'))
    {
    	if (token == "") continue;
    	if (pNode->m_children.contains(token))
			pNode = pNode->m_children[token].get();
    	else return {};
    }

    return *static_cast<T*>(pNode);
}

template<NodeType T>
OptionalRef<T> Node::FindFirstParentOfType()
{
	if (m_pOwner == nullptr) return {};
	if (dynamic_cast<T*>(m_pOwner)) return *static_cast<T*>(m_pOwner);

	return m_pOwner->FindFirstParentOfType<T>();
}

#include "Scripting/Proxies/NodeProxy.inl"

#endif // !NODE__H_

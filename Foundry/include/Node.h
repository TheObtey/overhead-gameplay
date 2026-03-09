#ifndef NODE__H_
#define NODE__H_

#include "Define.h"
#include "Event.hpp"
#include "SceneTree.h"
#include "Scripting/Lua/LuaScriptInstance.hpp"
#include "Serialization/ISerializable.h"
#include "Registries/AutomaticRegisterISerializable.hpp"

#include <functional>
#include <optional>
#include <string>
#include <sstream>
#include <vector>

class Node;
class SceneTree;
class SerializedObject;

template <typename T>
concept NodeType = std::is_base_of_v<Node, T>;

//Maybe move this in a global namespace
template <typename T>
using OptionalRef = std::optional<std::reference_wrapper<T>>;

//Base class off every node in the tree
class Node : public AutomaticRegisterISerializable<Node>, public ISerializable
{
public:
	class Proxy;

	virtual ~Node();

	virtual void OnUpdate(float delta) { };
	void Update(float delta);

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
	uint32 GetChildCount();

	template <NodeType T>
	T& GetNode(std::string const& path);

	//Only destroy the node if it has a parent
	void Destroy();
	virtual void Reparent(Node& newParent, bool keepGlobalTransform = true);
	void MoveChild(Node const& child, uint32 to);

	//override this method if the inherited node is not trivially copyable
	virtual std::unique_ptr<Node> Clone();
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;

	std::string GetName();
	Node* GetParent();
	SceneTree* GetSceneTree();

	//====Static Methods======
	template <NodeType T>
	static std::unique_ptr<T> CreateNode(std::string const& name);

	template <NodeType T>
	static void AttachScript(uptr<LuaScriptInstance>& script, T& node);

	static std::function<ISerializable*()> CreateInstance();
	static void Test() {};

	//====Event======
	Event<void(Node&)> OnSceneEnter;
	Event<void(Node&, float)> OnNodeUpdated;
	Event<void(Node&)> OnSceneLeave;
	Event<void(Node&)> OnParentChange;

protected:
	//private constructor for in-class initialization
	//====Constructors======
	Node() = delete;
	Node(std::string const& name);
	Node(Node const& other) = delete;
	Node(Node&& other) noexcept = delete;

	Node& operator=(Node const& other) = delete;
	Node& operator=(Node&& other) noexcept = delete;

	std::string m_name; //unique among siblings
	Node* m_pOwner = nullptr;
	SceneTree* m_pSceneTree = nullptr;

	uptr<Proxy> m_pProxy;
	uptr<LuaScriptInstance> m_pScriptInstance;

	std::unordered_map<std::string, std::unique_ptr<Node>> m_children{};
	std::vector<std::string> m_childrenOrder{};

private:
    void AttachChildImmediate(std::unique_ptr<Node>& child);

    friend class EngineServer;
	friend class unique_ptr;
};


//=== Templated function def ====
template <NodeType T>
std::unique_ptr<T> Node::CreateNode(std::string const& name)
{
    struct concrete_Node: public T {
        concrete_Node(std::string const& name) : T(name) {}
    };

	uptr<concrete_Node> ptr = std::make_unique<concrete_Node>(name);
	ptr->m_pProxy = std::make_unique<typename T::Proxy>(*ptr);
	auto _ = ptr->register_object;

    return std::move(ptr);
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
T& Node::GetNode(std::string const& path)
{
    std::stringstream ss(path);
    std::string token;

    //TODO Add this when scene tree is done
    //Node* pNode = path[0] == '/' ? m_pSceneTree.root : this;

    Node* pNode = this;
    while(std::getline(ss, token, '/'))
    {
        pNode = pNode->m_children[token].get();
    }

    return *static_cast<T*>(pNode);
}

#include "Scripting/Proxies/NodeProxy.inl"

#endif // !NODE__H_

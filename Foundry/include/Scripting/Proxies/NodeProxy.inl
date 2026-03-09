#include "Event.hpp"
#include "Scripting/Binder.h"
#include "Registries/AutomaticRegisterProxy.hpp"

#include <string>

#define BindProxy(P, X) struct P::ProxyBinding  : AutomaticRegisterProxy<P::ProxyBinding> { static void Bind(Binder& binder) { X } };
#define BIND(F) &Proxy::F

/*A node proxy class used to communicate between the scripting language and the engine API.
The engine manage memory, scripting just get refs.
Should be scripting language agnostic.
*/
class Node::Proxy
{
public:
	static Proxy* CreateNodeProxy(std::string const& name);
	//Garbage collect node proxy in case lua loose reference to node
	static void GCNodeProxy(Proxy* node);

public:
	struct ProxyBinding;

	Proxy(Node& node);
	Proxy(Proxy const& other) = delete;
	Proxy(Proxy && other) noexcept;

	Proxy& operator=(Proxy const& other) = delete;
	Proxy& operator=(Proxy&& other) noexcept;

	~Proxy() = default;

	void AddChild(Proxy& child);
	void RemoveChild(Proxy& child);
	void RemoveChild(std::string const& name);
	Proxy* FindChild(std::string const& name);
	Proxy& GetChild(uint32 index);
	std::vector<Proxy*> GetChildren();
	uint32 GetChildCount();

	Proxy* GetNode(std::string const& path);

	void Destroy();
	void Reparent(Proxy& newParent, bool keepGlobalTransform = true);
	void MoveChild(Proxy& child, uint32 to);

	Proxy* Clone();

	std::string GetName();
	Proxy* GetParent();
	SceneTree* GetSceneTree();

	Event<void()> OnSceneEnter;
	Event<void(float)> OnUpdate;
	Event<void()> OnSceneLeave;

	operator Node&();

private:
	Node* m_pNode;
};

struct Node::Proxy::ProxyBinding : AutomaticRegisterProxy<ProxyBinding>
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNode", &Node::Proxy::CreateNodeProxy);
		binder.BindClass<Proxy>("node",
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			"AddChild", BIND(AddChild),
			"RemoveChild", OVERLOAD(Proxy, void, Proxy&)(BIND(RemoveChild)),
			"RemoveChild", OVERLOAD(Proxy, void, std::string const&)(BIND(RemoveChild)),
			"FindChild", BIND(FindChild),
			"GetChild", BIND(GetChild),
			"GetChildren", BIND(GetChildren),
			"GetChildCount", BIND(GetChildCount),
			"GetNode", BIND(GetNode),
			"Destroy", BIND(Destroy),
			"Reparent", BIND(Reparent),
			"MoveChild", BIND(MoveChild),
			"Clone", BIND(Clone),
			"GetName", BIND(GetName),
			"GetParent", BIND(GetParent),
			"GetSceneTree", BIND(GetSceneTree));
	};
};

inline static Node::Proxy::ProxyBinding NodeBinding {};

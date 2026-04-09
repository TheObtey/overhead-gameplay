#include "Event.hpp"
#include "Scripting/Binder.h"
#include "Scripting/UserData.h"
#include "Registries/AutomaticRegisterProxy.hpp"

#include <string>

#define BindProxy(P, X) struct P::ProxyBinding { static void Bind(Binder& binder) { X } };
#define BIND(F) &Proxy::F

class SceneTreeProxy;
/*A node proxy class used
 *to communicate between the scripting language and the engine API.
The engine manage memory, scripting just get refs.
Should be scripting language agnostic.
*/
class Node::Proxy
{
public:
	static Proxy* CreateNodeProxy(std::string const& name);
	static Proxy* LoadNode(std::string const& path);
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

	std::string GetName() const;
	Proxy* GetParent() const;
	bool HasParent() const;
	SceneTreeProxy* GetSceneTree() const;

	Event<void()> OnSceneEnter;
	Event<void(double)> OnUpdate;
	Event<void(double)> OnPhysicsUpdate;
	Event<void()> OnSceneLeave;

	operator Node&() const;

	Node* GetProxyOwner() const;

protected:
	UserData m_userData;

private:
	Node* m_pNode;
};

struct Node::Proxy::ProxyBinding
{
	static void Bind(Binder& binder);
};

REGISTER_PROXY(Node::Proxy::ProxyBinding, NodeProxy);

#include "Node.h"
#include "Nodes/Node2D.h"
#include "Nodes/Node3D.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeCollider.h"
#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "Servers/EngineServer.h"
#include "SceneTree.h"
#include "Nodes/NodeWindow.h"

class Node3D;
using Proxy = Node::Proxy;

Proxy* Proxy::CreateNodeProxy(std::string const& name)
{
	uptr<Node> node = Node::CreateNode<Node>(name);
	Proxy* const ptr = node->m_pProxy.get();
	EngineServer::RegisterUnattachedNode(node);
	return ptr;
}

Proxy* Proxy::LoadNode(std::string const& path)
{
	uptr<Node> node = Node::LoadNodeFromJSON<Node>(path);
	Proxy* const ptr = node->m_pProxy.get();
	EngineServer::RegisterUnattachedNode(node);
	return ptr;
}

void Proxy::GCNodeProxy(Proxy* nodeProxy)
{
	if (nodeProxy)
		EngineServer::UnregisterUnattachedNode(nodeProxy->m_pNode);
}

Proxy::Proxy(Node& node) : m_pNode(&node),
m_userData(ScriptingEngine::GetScriptEngine(), sol::new_table())
{
	node.OnSceneEnter			+= [&](Node& n){ OnSceneEnter();};
	node.OnNodeUpdated			+= [&](Node& n, double const dt){ OnUpdate(dt); };
	node.OnNodePhysicsUpdated   += [&](Node& n, double const dt){ OnPhysicsUpdate(dt); };
	node.OnSceneLeave			+= [&](Node& n){ OnSceneLeave(); };
}

Proxy::Proxy(Proxy&& other) noexcept
{
	m_pNode = other.m_pNode; 
	other.m_pNode = nullptr;
}

Proxy& Proxy::operator=(Proxy&& other) noexcept
{
	m_pNode = other.m_pNode; 
	other.m_pNode = nullptr; 
	return *this;
}

void Proxy::AddChild(Proxy& child){
	uptr<Node>& unode = EngineServer::GetUnattachedNode(child.m_pNode);
	m_pNode->AddChild(unode);
	EngineServer::UnregisterUnattachedNode(unode.get());
}

void Proxy::RemoveChild(Proxy& child)
{
	m_pNode->RemoveChild(child);
}

void Proxy::RemoveChild(std::string const& name)
{
	m_pNode->RemoveChild(name);
}

Proxy* Proxy::FindChild(std::string const& name)
{
	if (auto child  = m_pNode->FindChild(name))
		return child->get().m_pProxy.get();
	return nullptr;
}

Proxy& Proxy::GetChild(uint32 index)
{
	return *m_pNode->GetChild(index).m_pProxy.get();
}

std::vector<Proxy*> Proxy::GetChildren()
{
	std::vector<Proxy*> res;
	for (auto child : m_pNode->GetChildren())
		res.push_back(child.get().m_pProxy.get());

	return res;
}

uint32 Proxy::GetChildCount()
{
	return m_pNode->GetChildCount();
}

Proxy* Proxy::GetNode(std::string const& path)
{
	if (auto const node = m_pNode->GetNode<Node>(path))
		return node->get().m_pProxy.get();

	return nullptr;
}

void Proxy::Destroy()
{
	m_pNode->Destroy();
}

void Proxy::Reparent(Proxy& newParent, bool keepGlobalTransform)
{
	m_pNode->Reparent(newParent, keepGlobalTransform);
}

void Proxy::MoveChild(Proxy& child, uint32 to)
{
	m_pNode->MoveChild(child, to);
}

Proxy* Proxy::Clone()
{
	uptr<Node> node = m_pNode->Clone();
	Proxy* const ptr = node->m_pProxy.get();
	EngineServer::RegisterUnattachedNode(node);
	return ptr;
}

std::string Proxy::GetName() const
{
	return m_pNode->GetName(); 
}

Proxy* Proxy::GetParent() const
{
	return m_pNode->GetParent()->m_pProxy.get();
}

bool Proxy::HasParent() const
{
	return m_pNode->HasParent();
}

SceneTreeProxy* Proxy::GetSceneTree() const
{
	SceneTreeProxy* prox = &m_pNode->GetSceneTree()->m_proxy;
	return prox;
}

sol::object Proxy::As(NodeTypes const type, sol::this_state state)
{
	switch (type)
	{
	case NodeTypes::NODE:
		return sol::make_object_userdata(state, this);
	case NodeTypes::NODE3D:
		return sol::make_object_userdata(state, static_cast<Node3D::Proxy*>(this));
	case NodeTypes::NODE2D:
		return sol::make_object_userdata(state, static_cast<Node2D::Proxy*>(this));
	case NodeTypes::NODE_CAMERA:
		return sol::make_object_userdata(state, static_cast<NodeCamera::Proxy*>(this));
	case NodeTypes::NODE_MESH:
		return sol::make_object_userdata(state, static_cast<NodeMesh::Proxy*>(this));
	case NodeTypes::NODE_ANIMATED_MESH:
		UNIMPLEMENTED
	case NodeTypes::NODE_COLLIDER:
		return sol::make_object_userdata(state, static_cast<NodeCollider::Proxy*>(this));
	case NodeTypes::NODE_RIGIDBODY:
		return sol::make_object_userdata(state, static_cast<NodeRigidBody::Proxy*>(this));
	case NodeTypes::NODE_VIEWPORT:
		return sol::make_object_userdata(state, static_cast<NodeViewport::Proxy*>(this));
	case NodeTypes::NODE_VISUAL:
		return sol::make_object_userdata(state, static_cast<NodeVisual::Proxy*>(this));
	case NodeTypes::NODE_WINDOW:
		return sol::make_object_userdata(state, static_cast<NodeWindow::Proxy*>(this));
	}
}

bool Proxy::Is(NodeTypes type)
{
	switch (type)
	{
	case NodeTypes::NODE:
		return true;
	case NodeTypes::NODE3D:
		return dynamic_cast<Node3D*>(m_pNode) != nullptr;
	case NodeTypes::NODE2D:
		return dynamic_cast<Node2D*>(m_pNode) != nullptr;
	case NodeTypes::NODE_CAMERA:
		return dynamic_cast<NodeCamera*>(m_pNode) != nullptr;
	case NodeTypes::NODE_MESH:
		return dynamic_cast<NodeMesh*>(m_pNode) != nullptr;
	case NodeTypes::NODE_ANIMATED_MESH:
		UNIMPLEMENTED
	case NodeTypes::NODE_COLLIDER:
		return dynamic_cast<NodeCollider*>(m_pNode) != nullptr;
	case NodeTypes::NODE_RIGIDBODY:
		return dynamic_cast<NodeRigidBody*>(m_pNode) != nullptr;
	case NodeTypes::NODE_VIEWPORT:
		return dynamic_cast<NodeViewport*>(m_pNode) != nullptr;
	case NodeTypes::NODE_VISUAL:
		return dynamic_cast<NodeVisual*>(m_pNode) != nullptr;
	case NodeTypes::NODE_WINDOW:
		return dynamic_cast<NodeWindow*>(m_pNode) != nullptr;
	}
}

Proxy::operator Node&() const
{
	return *m_pNode;
}

Node* Proxy::GetProxyOwner() const
{
	return m_pNode;
}

void Proxy::ProxyBinding::Bind(Binder &binder)
{
	binder.BindEnum<NodeTypes>("NodeTypes",
		"NODE", NodeTypes::NODE,
		"NODE3D", NodeTypes::NODE3D,
		"NODE2D", NodeTypes::NODE2D,
		"NODE_CAMERA", NodeTypes::NODE_CAMERA,
		"NODE_MESH", NodeTypes::NODE_MESH,
		"NODE_ANIMATED_MESH", NodeTypes::NODE_ANIMATED_MESH,
		"NODE_COLLIDER", NodeTypes::NODE_COLLIDER,
		"NODE_RIGIDBODY", NodeTypes::NODE_RIGIDBODY,
		"NODE_VIEWPORT", NodeTypes::NODE_VIEWPORT,
		"NODE_VISUAL", NodeTypes::NODE_VISUAL,
		"NODE_WINDOW", NodeTypes::NODE_WINDOW
	);

	binder.BindFunction("CreateNode", &Node::Proxy::CreateNodeProxy);
	binder.BindFunction("LoadNode", &Node::Proxy::LoadNode);
	binder.BindClass<Proxy>("node",
		sol::meta_function::garbage_collect, BIND(GCNodeProxy),
		sol::meta_function::new_index, StoreUserData(),
		sol::meta_function::index, LoadUserData(),
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
		"HasParent", BIND(HasParent),
		"GetSceneTree", BIND(GetSceneTree),
		"As", BIND(As),
		"Is", BIND(Is)
	);
}

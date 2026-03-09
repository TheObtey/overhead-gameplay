#include "Servers/EngineServer.h"
#include "Node.h"

using Proxy = Node::Proxy;

Proxy* Proxy::CreateNodeProxy(std::string const& name)
{
	uptr<Node> node = Node::CreateNode<Node>(name);
	Proxy* const ptr = node->m_pProxy.get();
	EngineServer::RegisterUnattachedNode(node);
	return ptr;
}

void Proxy::GCNodeProxy(Proxy* nodeProxy)
{
	if (nodeProxy)
		EngineServer::UnregisterUnattachedNode(nodeProxy->m_pNode);
}

Proxy::Proxy(Node& node) : m_pNode(&node)  
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
	//TODO MAKE WORK FALL ALL NODE TYPE
	//ALSO ERROR HANDLING ?
	return m_pNode->GetNode<Node>(path).m_pProxy.get();
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

std::string Proxy::GetName()
{ 
	return m_pNode->GetName(); 
}

Proxy* Proxy::GetParent()
{
	return m_pNode->m_pProxy.get();
}

SceneTree* Proxy::GetSceneTree()
{
	return m_pNode->GetSceneTree();
}

Proxy::operator Node&()
{
	return *m_pNode;
}

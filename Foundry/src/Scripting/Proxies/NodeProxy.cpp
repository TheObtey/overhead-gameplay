#include "Servers/LuaServer.h"
#include "Node.h"

using Proxy = Node::Proxy;


Proxy Proxy::CreateNodeProxy(std::string const& name)
{
	uptr<Node> node = Node::CreateNode<Node>(name);
	Node::Proxy proxy(*node);
	LuaServer::RegisterUnattachedNode(node);
	return proxy;
}

void Proxy::GCNodeProxy(Proxy *nodeProxy)
{
	if (nodeProxy) 
		LuaServer::UnregisterUnattachedNode(nodeProxy->m_pNode);
}

Proxy::Proxy(Node& node) : m_pNode(&node)  
{
	node.OnSceneEnter  += [&](Node& node){ OnSceneEnter();};
	node.OnNodeUpdated += [&](Node& node, float dt){ OnUpdate(dt); };
	node.OnSceneLeave  += [&](Node& node){ OnSceneLeave(); };
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

void Proxy::AddChild(Proxy* child){ 
	uptr<Node>& unode = LuaServer::GetUnattachedNode(child->m_pNode);
	m_pNode->AddChild(unode);
	LuaServer::UnregisterUnattachedNode(unode.get());
}

std::string Proxy::GetName()
{ 
	return m_pNode->GetName(); 
}




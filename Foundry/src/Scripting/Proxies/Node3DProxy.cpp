#include "Servers/EngineServer.h"
#include "Nodes/Node3D.h"

using Proxy = Node3D::Proxy;

Proxy* Proxy::CreateNode3DProxy(std::string const& name)
{
	uptr<Node3D> node = Node::CreateNode<Node3D>(name);
	Proxy* ptr = static_cast<Proxy*>(node->m_pProxy.get());
	uptr<Node> nodeRigidBody = std::move(node);
	EngineServer::RegisterUnattachedNode(nodeRigidBody);
	return ptr;
}


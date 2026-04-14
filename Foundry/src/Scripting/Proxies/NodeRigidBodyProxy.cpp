#include "Servers/EngineServer.h"
#include "Nodes/NodeRigidBody.h"

using Proxy = NodeRigidBody::Proxy;

Proxy* Proxy::CreateNodeRigidBodyProxy(std::string const& name)
{
	uptr<NodeRigidBody> node = Node::CreateNode<NodeRigidBody>(name);
	Proxy* ptr = static_cast<Proxy*>(node->m_pProxy.get());
	uptr<Node> nodeRigidBody = std::move(node);
	EngineServer::RegisterUnattachedNode(nodeRigidBody);
	return ptr;
}

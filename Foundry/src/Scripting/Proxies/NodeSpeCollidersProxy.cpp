#include "Servers/EngineServer.h"
#include "Nodes/NodeCollider.h"

using ProxyB = NodeBoxCollider::Proxy;

ProxyB* ProxyB::CreateNodeBoxColliderProxy(std::string const& name)
{
	uptr<NodeBoxCollider> node = Node::CreateNode<NodeBoxCollider>(name);
	ProxyB* ptr = static_cast<ProxyB*>(node->m_pProxy.get());
	uptr<Node> nodecollider = std::move(node);
	EngineServer::RegisterUnattachedNode(nodecollider);
	return ptr;
}

using ProxyS = NodeSphereCollider::Proxy;

ProxyS* ProxyS::CreateNodeSphereColliderProxy(std::string const& name)
{
	uptr<NodeSphereCollider> node = Node::CreateNode<NodeSphereCollider>(name);
	ProxyS* ptr = static_cast<ProxyS*>(node->m_pProxy.get());
	uptr<Node> nodecollider = std::move(node);
	EngineServer::RegisterUnattachedNode(nodecollider);
	return ptr;
}

using ProxyC = NodeCapsuleCollider::Proxy;

ProxyC* ProxyC::CreateNodeCapsuleColliderProxy(std::string const& name)
{
	uptr<NodeCapsuleCollider> node = Node::CreateNode<NodeCapsuleCollider>(name);
	ProxyC* ptr = static_cast<ProxyC*>(node->m_pProxy.get());
	uptr<Node> nodecollider = std::move(node);
	EngineServer::RegisterUnattachedNode(nodecollider);
	return ptr;
}
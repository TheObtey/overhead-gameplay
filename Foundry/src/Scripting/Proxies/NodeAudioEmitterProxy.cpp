#include "Nodes/NodeAudioEmitter.h"
#include "Registries/AutomaticRegisterProxy.hpp"
#include "Servers/EngineServer.h"

using Proxy = NodeAudioEmitter::Proxy;

Proxy* Proxy::CreateNodeAudioEmitterProxy(std::string const& name)
{
	uptr<NodeAudioEmitter> node = Node::CreateNode<NodeAudioEmitter>(name);
	Proxy* ptr = static_cast<Proxy*>(node->m_pProxy.get());
	uptr<Node> nodeAudioEmitter = std::move(node);
	EngineServer::RegisterUnattachedNode(nodeAudioEmitter);
	return ptr;
}

//REGISTER_PROXY(Proxy::ProxyBinding, NodeAudioEmitterProxy);
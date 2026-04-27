#include "Nodes/NodeAudioListener.h"
#include "Registries/AutomaticRegisterProxy.hpp"
#include "Servers/EngineServer.h"

using Proxy = NodeAudioListener::Proxy;

Proxy* Proxy::CreateNodeAudioListenerProxy(std::string const& name)
{
	uptr<NodeAudioListener> node = Node::CreateNode<NodeAudioListener>(name);
	Proxy* ptr = static_cast<Proxy*>(node->m_pProxy.get());
	uptr<Node> nodeNodeAudioListener = std::move(node);
	EngineServer::RegisterUnattachedNode(nodeNodeAudioListener);
	return ptr;
}

//REGISTER_PROXY(Proxy::ProxyBinding, NodeAudioListenerProxy);
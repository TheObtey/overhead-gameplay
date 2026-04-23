#include <Nodes/NodeAudioListener.h>

class NodeAudioListener::Proxy : public Node::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node::Proxy(node), m_pNode(static_cast<NodeAudioListener*>(&node)) {}

	static Proxy* CreateNodeAudioListenerProxy(std::string const& name);

	void SetListenerPosition(glm::vec3 position) { return m_pNode->SetListenerPosition(position); }
	glm::vec3 GetListenerPosition() { return m_pNode->GetListenerPosition(); }

	void SetListenerDirection(glm::vec3 position) { return m_pNode->SetListenerDirection(position); }
	glm::vec3 GetListenerDirection() { return m_pNode->GetListenerDirection(); }

private:
	NodeAudioListener* m_pNode;
};

struct NodeAudioListener::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNodeAudioListener", &NodeAudioListener::Proxy::CreateNodeAudioListenerProxy);
		binder.BindClass<NodeAudioListener::Proxy>("nodeaudiolistener",
			sol::base_classes, sol::bases<Node::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			"SetListenerPosition", BIND(SetListenerPosition),
			"GetListenerPosition", BIND(GetListenerPosition),
			"SetListenerDirection", BIND(SetListenerDirection),
			"GetListenerDirection", BIND(GetListenerDirection)
		);
	};
};

//BindProxy(NodeAudioListener::Proxy,
//	return binder.BindClass<NodeAudioListener::Proxy>("nodeaudiolistener",
//		"SetListenerPosition", BIND(SetListenerPosition),
//		"GetListenerPosition", BIND(GetListenerPosition),
//		"SetListenerDirection", BIND(SetListenerDirection),
//		"GetListenerDirection", BIND(GetListenerDirection)
//	);
//)

//REGISTER_PROXY(NodeAudioListener::Proxy::ProxyBinding, NodeAudioListenerProxy);
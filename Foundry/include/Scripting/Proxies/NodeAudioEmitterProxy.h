#include <Nodes/NodeAudioEmitter.h>

class NodeAudioEmitter::Proxy : public Node::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node::Proxy(node), m_pNode(static_cast<NodeAudioEmitter*>(&node)) {}

	static Proxy* CreateNodeAudioEmitterProxy(std::string const& name);

	bool Load(std::string const& filePath, AudioChannel* channel) { return m_pNode->Load(filePath, channel); }

	void Play() { return m_pNode->Play(); }
	void Stop() { return m_pNode->Stop(); }
	void SetLoop(bool isLooping) { return m_pNode->SetLoop(isLooping); }

	void SetSourcePosition(glm::vec3 position) { return m_pNode->SetSourcePosition(position); }
	glm::vec3 GetSourcePosition() { return m_pNode->GetSourcePosition(); }
	void SetSourceDirection(glm::vec3 position) { return m_pNode->SetSourceDirection(position); }
	glm::vec3 GetSourceDirection() { return m_pNode->GetSourceDirection(); }

	bool IsPlaying() { return m_pNode->IsPlaying(); }

private:
	NodeAudioEmitter* m_pNode;
};

struct NodeAudioEmitter::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNodeAudioEmitter", &NodeAudioEmitter::Proxy::CreateNodeAudioEmitterProxy);
		binder.BindClass<NodeAudioEmitter::Proxy>("nodeaudioemitter",
			sol::base_classes, sol::bases<Node::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			"Load", BIND(Load),
			"Play", BIND(Play),
			"Stop", BIND(Stop),
			"SetLoop", BIND(SetLoop),
			"SetSourcePosition", BIND(SetSourcePosition),
			"GetSourcePosition", BIND(GetSourcePosition),
			"SetSourceDirection", BIND(SetSourceDirection),
			"GetSourceDirection", BIND(GetSourceDirection),
			"IsPlaying", BIND(IsPlaying)
		);
	};
};


//struct NodeAudioEmitter::Proxy::ProxyBinding
//{
//	static void Bind(Binder& binder);
//};
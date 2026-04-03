
class NodeAudioEmitter::Proxy : public Node::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node::Proxy(node), m_pNode(static_cast<NodeAudioEmitter*>(&node)) {}

	bool Load(std::string const& filePath, AudioChannel* channel) { return m_pNode->Load(filePath, channel); }

	void Play() { return m_pNode->Play(); }
	void Stop() { return m_pNode->Stop(); }
	void SetLoop(bool isLooping) { return m_pNode->SetLoop(isLooping); }

	void SetSourcePosition(glm::vec3 position) { return m_pNode->SetSourcePosition(position); }
	glm::vec3 GetSourcePosition() { return m_pNode->GetSourcePosition(); }

	bool IsPlaying() { return m_pNode->IsPlaying(); }

private:
	NodeAudioEmitter* m_pNode;
};

BindProxy(NodeAudioEmitter::Proxy,
	return binder.BindClass<NodeAudioEmitter::Proxy>("nodeaudioemitter",
		"Load", BIND(Load),
		"Play", BIND(Play),
		"Stop", BIND(Stop),
		"SetLoop", BIND(SetLoop),
		"SetSourcePosition", BIND(SetSourcePosition),
		"GetSourcePosition", BIND(GetSourcePosition),
		"IsPlaying", BIND(IsPlaying)
	);
)

REGISTER_PROXY(NodeAudioEmitter::Proxy::ProxyBinding, NodeAudioEmitterProxy);
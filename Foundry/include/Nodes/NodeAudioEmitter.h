#ifndef FOUNDRY_NODEAUDIOEMITTER__H_
#define FOUNDRY_NODEAUDIOEMITTER__H_

#include "Node.h"
#include "Servers/AudioServer.h"
#include "Transform3D.h"

// Node that play and manage audio

class NodeAudioEmitter : public Node
{
public:
	class Proxy;

	NodeAudioEmitter(std::string const& name);
	~NodeAudioEmitter() override;

	bool Load(const std::string& filePath, AudioChannel* channel);

	void Play();
	void Stop();
	void SetLoop(bool isLooping);

	void SetSourcePosition(glm::vec3 position);
	glm::vec3 GetSourcePosition();

	void SetSourceDirection(glm::vec3 position);
	glm::vec3 GetSourceDirection();

	bool IsPlaying() { return m_isPlaying; };

	virtual void OnUpdate(double delta) override;

	static ISerializable* CreateInstance();

protected:
	ma_sound m_sound{};
	bool m_isPlaying = false;
};

REGISTER_ISERIALIZABLE(NodeAudioEmitter, NodeAudioEmitter::CreateInstance);

#include "Scripting/Proxies/NodeAudioEmitterProxy.inl"

#endif
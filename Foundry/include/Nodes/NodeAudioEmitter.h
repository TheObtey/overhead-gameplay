#ifndef FOUNDRY_NODEAUDIOEMITTER__H_
#define FOUNDRY_NODEAUDIOEMITTER__H_

#include "Node.h"
#include "Servers/AudioServer.h"

class NodeAudioEmitter : public Node
{
public:
	NodeAudioEmitter(std::string const& name);
	~NodeAudioEmitter() override = default;

	bool Load(const char* filePath, AudioChannel channel);

	void Play();
	void Stop();
	void AudioSetLoop(bool value);

	virtual void OnUpdate(double delta) override;

	static ISerializable* CreateInstance();

protected:
	ma_sound m_sound{};
	bool m_isPlaying = false;
private:

};

REGISTER_ISERIALIZABLE(NodeAudioEmitter, NodeAudioEmitter::CreateInstance);

#endif
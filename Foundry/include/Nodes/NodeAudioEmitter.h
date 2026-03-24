#ifndef FOUNDRY_NODEAUDIOEMITTER__H_
#define FOUNDRY_NODEAUDIOEMITTER__H_

#include "Node.h"
#include "AudioEngine.h"

class NodeAudioEmitter : public Node
{
public:
	NodeAudioEmitter(std::string const& name);
	~NodeAudioEmitter() override = default;

	void PlayAudio(const char* filePath);

	static ISerializable* CreateInstance();

protected:
	AudioEngine m_audio;

private:

};

REGISTER_ISERIALIZABLE(NodeAudioEmitter, NodeAudioEmitter::CreateInstance);

#endif
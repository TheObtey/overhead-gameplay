#ifndef FOUNDRY_NODEAUDIOEMITTER__H_
#define FOUNDRY_NODEAUDIOEMITTER__H_

#include "Node.h"
#include "Servers/AudioServer.h"

class NodeAudioEmitter : public Node
{
public:
	NodeAudioEmitter(std::string const& name);
	~NodeAudioEmitter() override = default;

	void AudioSetFile(const char* filePath);

	void AudioPlay();
	void AudioStop();
	void AudioSetLoop(bool value);

	virtual void OnUpdate(double delta) override;

	static ISerializable* CreateInstance();

protected:

private:

};

REGISTER_ISERIALIZABLE(NodeAudioEmitter, NodeAudioEmitter::CreateInstance);

#endif
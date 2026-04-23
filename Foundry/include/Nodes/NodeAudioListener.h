#ifndef FOUNDRY_NODEAUDIOLISTENER__H_
#define FOUNDRY_NODEAUDIOLISTENER__H_

#include "Node.h"
#include "Servers/AudioServer.h"
#include "Transform3D.h"

// Node that can listen to audio that emit an AudioEmitter

class NodeAudioListener : public Node
{
public:
	class Proxy;

	NodeAudioListener(std::string const& name);
	~NodeAudioListener() override;

	void SetListenerPosition(glm::vec3 position);
	glm::vec3 GetListenerPosition();

	void SetListenerDirection(glm::vec3 position);
	glm::vec3 GetListenerDirection();

	virtual void OnUpdate(double delta) override;

	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;

	static ISerializable* CreateInstance();

protected:
	uint32 m_listenerIndex;
};

REGISTER_ISERIALIZABLE(NodeAudioListener, NodeAudioListener::CreateInstance);

#include "Scripting/Proxies/NodeAudioListenerProxy.h"

#endif
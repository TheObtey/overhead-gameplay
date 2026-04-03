#include "Nodes/NodeAudioListener.h"

NodeAudioListener::NodeAudioListener(std::string const& name) : Node(name)
{
	m_listenerIndex = AudioServer::AllocateListenerIndex();
}

NodeAudioListener::~NodeAudioListener()
{
	AudioServer::ReleaseListenerIndex(m_listenerIndex);
}

void NodeAudioListener::SetListenerPosition(glm::vec3 position)
{
	ma_engine& soundEngine = AudioServer::GetSoundEngine();

	ma_engine_listener_set_position(&soundEngine, m_listenerIndex, position.x, position.y, position.z);
}

glm::vec3 NodeAudioListener::GetListenerPosition()
{
	ma_engine& soundEngine = AudioServer::GetSoundEngine();

	ma_vec3f posMa = ma_engine_listener_get_position(&soundEngine, m_listenerIndex);
	glm::vec3 posGlm = { posMa.x, posMa.y, posMa.z };

	return posGlm; 
}

void NodeAudioListener::SetListenerDirection(glm::vec3 position)
{
	ma_engine& soundEngine = AudioServer::GetSoundEngine();

	ma_engine_listener_set_direction(&soundEngine, m_listenerIndex, position.x, position.y, position.z);
}

glm::vec3 NodeAudioListener::GetListenerDirection()
{
	ma_engine& soundEngine = AudioServer::GetSoundEngine();

	ma_vec3f posMa = ma_engine_listener_get_direction(&soundEngine, m_listenerIndex);
	glm::vec3 posGlm = { posMa.x, posMa.y, posMa.z };

	return posGlm;
}

void NodeAudioListener::OnUpdate(double delta)
{
	Node::OnUpdate(delta);
}

ISerializable* NodeAudioListener::CreateInstance()
{
	return CreateNode<NodeAudioListener>("NodeAudioListener").release();
}
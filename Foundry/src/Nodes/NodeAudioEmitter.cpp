#include "Nodes/NodeAudioEmitter.h"

NodeAudioEmitter::NodeAudioEmitter(std::string const& name) : Node(name)
{
}

bool NodeAudioEmitter::Load(const char* filePath, AudioChannel* channel)
{
    ma_engine& soundEngine = AudioServer::GetSoundEngine();

    if (ma_sound_init_from_file(&soundEngine, filePath, 0, &channel->soundGroup, NULL, &m_sound) != MA_SUCCESS)
    {
        printf("Failed to load sound: %s\n", filePath);
        return false;
    }

    return true;
}

void NodeAudioEmitter::Play()
{
    if (!m_isPlaying) 
    {
        ma_sound_start(&m_sound);
        m_isPlaying = true;
    } 
}

void NodeAudioEmitter::Stop()
{
    if (m_isPlaying)
    {
        ma_sound_stop(&m_sound);
        m_isPlaying = false;
    }
}

void NodeAudioEmitter::AudioSetLoop(bool value)
{
	//AudioServer::SetLoop(value);
}

void NodeAudioEmitter::OnUpdate(double delta)
{
	Node::OnUpdate(delta);
}

ISerializable* NodeAudioEmitter::CreateInstance()
{
	return CreateNode<NodeAudioEmitter>("NodeAudioEmitter").release();
}
#include "Nodes/NodeAudioEmitter.h"
#include "Serialization/SerializeObject.hpp"

NodeAudioEmitter::NodeAudioEmitter(std::string const& name) : Node(name)
{
}
NodeAudioEmitter::~NodeAudioEmitter()
{
    ma_sound_uninit(&m_sound);
}

bool NodeAudioEmitter::Load(std::string const& filePath, AudioChannel* channel)
{
    ma_engine& soundEngine = AudioServer::GetSoundEngine();

    if (ma_sound_init_from_file(&soundEngine, filePath.c_str(), 0, &channel->GetGroup(), NULL, &m_sound) != MA_SUCCESS)
    {
        Logger::Log("Failed to load sound: " + filePath);
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

void NodeAudioEmitter::SetLoop(bool isLooping)
{
    ma_sound_set_looping(&m_sound, isLooping);
}

void NodeAudioEmitter::SetSourcePosition(glm::vec3 position)
{
    ma_sound_set_position(&m_sound, position.x, position.y, position.z);
}

glm::vec3 NodeAudioEmitter::GetSourcePosition()
{
    ma_vec3f posMa = ma_sound_get_position(&m_sound);
    glm::vec3 posGlm = { posMa.x, posMa.y, posMa.z };
    return posGlm;
}

void NodeAudioEmitter::SetSourceDirection(glm::vec3 position)
{
    ma_sound_set_direction(&m_sound, position.x, position.y, position.z);
}

glm::vec3 NodeAudioEmitter::GetSourceDirection()
{
    ma_vec3f dirMa = ma_sound_get_direction(&m_sound);
    glm::vec3 posGlm = { dirMa.x, dirMa.y, dirMa.z };
    return posGlm;
}

void NodeAudioEmitter::OnUpdate(double delta)
{
	Node::OnUpdate(delta);
}

void NodeAudioEmitter::Serialize(SerializedObject& datas) const
{
    Node::Serialize(datas);
    datas.SetType("NodeAudioEmitter");
    //datas.AddPublicElement("Transform", static_cast<ISerializable const*>(&m_transform));
}


void NodeAudioEmitter::Deserialize(SerializedObject const& datas)
{
    Node::Deserialize(datas);
    //datas.GetPublicElement("Transform", static_cast<ISerializable*>(&m_transform));
}

ISerializable* NodeAudioEmitter::CreateInstance()
{
	return CreateNode<NodeAudioEmitter>("NodeAudioEmitter").release();
}
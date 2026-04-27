#define MINIAUDIO_IMPLEMENTATION

#include "Servers/AudioServer.h"

bool AudioServer::Initialize()
{
    Instance().m_soundEngineConfig = ma_engine_config_init();
    Instance().m_soundEngineConfig.listenerCount = MA_ENGINE_MAX_LISTENERS;
    
    if (ma_engine_init(&Instance().m_soundEngineConfig, &Instance().m_soundEngine) != MA_SUCCESS)
    {
        Logger::Log("Failed to init audio engine");
        return false;
    }

    Instance().m_channels.reserve(32);
    return true;
}

void AudioServer::Shutdown()
{
    for (auto& channel : Instance().m_channels)
    {
        ma_sound_group_uninit(&channel->soundGroup);
    }
    Instance().m_channels.clear();
    ma_engine_uninit(&Instance().m_soundEngine);
}

AudioChannel* AudioServer::GetChannel(std::string const& name)
{
    for (auto& channel : Instance().m_channels)
    {
        if (channel->name == name) return channel.get();
    }
    return nullptr;
}

AudioChannel* AudioServer::CreateChannel(std::string const& name)
{
    if (GetChannel(name) != nullptr) { return GetChannel(name); }

    uptr<AudioChannel> newChannel = std::make_unique<AudioChannel>();
    newChannel->name = name;

    if (ma_sound_group_init(&Instance().m_soundEngine, 0, NULL, &newChannel->soundGroup) != MA_SUCCESS)
    {
        Logger::Log("Failed to create sound group: " + name);
        return nullptr;
    }

    GetChannels().push_back(std::move(newChannel));
    return GetChannels().back().get();
}

uint32 AudioServer::AllocateListenerIndex()
{
    if (!Instance().m_availableListenerIndex.empty())
    {
        uint32 index = Instance().m_availableListenerIndex.back();
        Instance().m_availableListenerIndex.pop_back();
        return index;
    }
    if (Instance().m_nextListenerIndex >= Instance().m_soundEngineConfig.listenerCount)
    {
        Logger::Log("Warning: max listeners reached " + Instance().m_soundEngineConfig.listenerCount);
        return 0;
    }

    return Instance().m_nextListenerIndex++;
}

void AudioServer::ReleaseListenerIndex(uint32 index)
{
    Instance().m_availableListenerIndex.push_back(index);
}

void AudioServer::SetMasterVolume(float volume)
{
    volume = std::clamp(volume, 0.0f, 1.0f);

    AudioServer& self = Instance();
    self.m_masterVolume = volume;
    ma_engine_set_volume(&self.m_soundEngine, volume);
}

void AudioServer::SetGroupVolume(AudioChannel* channel,float volume)
{
    volume = std::clamp(volume, 0.0f, 1.0f);

    ma_sound_group_set_volume(&channel->soundGroup, volume);
}
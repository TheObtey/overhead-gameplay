#define MINIAUDIO_IMPLEMENTATION

#include "Servers/AudioServer.h"

bool AudioServer::Init()
{
    if (ma_engine_init(NULL, &Instance().m_soundEngine) != MA_SUCCESS)
    {
        printf("Failed to init audio engine\n");
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

    ma_engine_uninit(&Instance().m_soundEngine);
}

AudioChannel* AudioServer::GetChannel(const std::string& name)
{
    for (auto& channel : Instance().m_channels)
    {
        if (channel->name == name) return channel;
    }

    return nullptr;
}

AudioChannel* AudioServer::CreateChannel(const std::string& name) 
{
    if (GetChannel(name) != nullptr) { return GetChannel(name); }

    AudioChannel* newChannel = new AudioChannel(); // allocation sur le heap
    newChannel->name = name;

    if (ma_sound_group_init(&Instance().m_soundEngine, 0, NULL, &newChannel->soundGroup) != MA_SUCCESS)
    {
        printf("Failed to create sound group: %s\n", name.c_str());
        delete newChannel;
        return nullptr;
    }

    GetChannels().push_back(newChannel);

    return newChannel;
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
#ifndef AUDIO_SERVER__H_
#define AUDIO_SERVER__H_

#include "Server.hpp"
#include "Node.h"
#include "Logger.hpp"

#include "Define.h"
#include "miniaudio.h"

#include <iostream>

struct AudioChannel 
{
	std::string name;
	ma_sound_group soundGroup;
};

// Manage the complete audio system using channels. (Required when using any related sound Classes/Nodes)

class AudioServer : public Server<AudioServer>
{
public:
	AudioServer() = default;
	~AudioServer() = default;
	
	static bool Init();
	static void Shutdown();

	static AudioChannel* GetChannel(const std::string& name);
	static AudioChannel* CreateChannel(const std::string& name);

	static ma_uint32 AllocateListenerIndex();
	static void ReleaseListenerIndex(ma_uint32 index);

	static void SetMasterVolume(float volume);
	static void SetGroupVolume(AudioChannel* channel, float volume);

	static float GetMasterVolume() { return Instance().m_masterVolume; };
	static float GetGroupVolume(AudioChannel channel) { return ma_sound_group_get_volume(&channel.soundGroup); };

	static ma_engine& GetSoundEngine() { return Instance().m_soundEngine; };
	static std::vector<AudioChannel*>& GetChannels() { return Instance().m_channels; };

protected:
	ma_engine m_soundEngine;
	ma_engine_config m_soundEngineConfig;
	std::vector<AudioChannel*> m_channels;
	std::vector<ma_uint32> m_availableListenerIndex;
	ma_uint32 m_nextListenerIndex = 0;
	float m_masterVolume = 1.f;

private:
	void FlushCommandsImpl() override {};
	void BuildTasksImpl(TaskGraph& graph) override {};
	void OnInitialize() override {}
};

#endif
#ifndef AUDIO_SERVER__H_
#define AUDIO_SERVER__H_

#include "Server.hpp"
#include "Node.h"
#include "Logger.hpp"

#include "Define.h"
#include "miniaudio.h"

#include <iostream>

class AudioChannel 
{
public:
	std::string const& GetName() const { return name; };
	ma_sound_group& GetGroup() { return soundGroup; };

private:
	std::string name;
	ma_sound_group soundGroup;

	friend class AudioServer;
};

// Manage the complete audio system using channels. (Required when using any related sound Classes/Nodes)

class AudioServer : public Server<AudioServer>
{
public:
	AudioServer() = default;
	~AudioServer() = default;
	
	static bool Initialize();
	static void Shutdown();

	static AudioChannel* GetChannel(std::string const& name);
	static AudioChannel* CreateChannel(std::string const& name);

	static uint32 AllocateListenerIndex();
	static void ReleaseListenerIndex(uint32 index);

	static void SetMasterVolume(float volume);
	static void SetGroupVolume(AudioChannel* channel, float volume);

	static float GetMasterVolume() { return Instance().m_masterVolume; };
	static float GetGroupVolume(AudioChannel& channel) { return ma_sound_group_get_volume(&channel.soundGroup); };

	static ma_engine& GetSoundEngine() { return Instance().m_soundEngine; };
	static std::vector<uptr<AudioChannel>>& GetChannels() { return Instance().m_channels; };

protected:
	ma_engine m_soundEngine;
	ma_engine_config m_soundEngineConfig;
	std::vector<uptr<AudioChannel>> m_channels;
	std::vector<uint32> m_availableListenerIndex;
	uint32 m_nextListenerIndex = 0;
	float m_masterVolume = 1.f;

private:
	void FlushCommandsImpl() override {};
	void BuildTasksImpl(TaskGraph& graph) override {};
	void OnInitialize() override {};
	void OnUnInitialize() override {};

	friend Server<AudioServer>;
};

#endif
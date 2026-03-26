#ifndef AUDIO_SERVER__H_
#define AUDIO_SERVER__H_

#include "Server.hpp"
#include "Node.h"

#include "Define.h"
#include "miniaudio.h"

#include <iostream>

struct AudioChannel 
{
	std::string name;
	ma_sound_group soundGroup;
};

class AudioServer : public Server<AudioServer>
{
public:
	AudioServer() = default;
	~AudioServer() = default;
	
	static bool Init();
	static void Shutdown();

	static AudioChannel* GetChannel(const std::string& name);
	static AudioChannel* CreateChannel(const std::string& name);

	static void SetMasterVolume(float volume);
	static void SetGroupVolume(AudioChannel channel, float volume);

	static float GetMasterVolume() { return ma_engine_get_volume(&Instance().m_soundEngine); };
	//static float GetGroupVolume(AudioChannel channel) { return ma_sound_group_get_volume(&channel.soundGroup); };

	static ma_engine& GetSoundEngine() { return Instance().m_soundEngine; };
	static std::vector<AudioChannel*>& GetChannels() { return Instance().m_channels; };

protected:
	ma_engine m_soundEngine{};
	std::vector<AudioChannel*> m_channels;

private:
	void FlushCommandsImpl() override {};
	void BuildTasksImpl(TaskGraph& graph) override {};
	void OnInitialize() override {}
};

#endif
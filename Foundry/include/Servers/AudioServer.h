#ifndef AUDIO_SERVER__H_
#define AUDIO_SERVER__H_

#include "Server.hpp"
#include "Node.h"

#include "Define.h"
#include "miniaudio.h"

#include <iostream>

class AudioServer : public Server<AudioServer>
{
public:
	AudioServer() = default;
	~AudioServer() = default;
	
	static void SetAudioFile(const char* filePath);

	static void Play();
	static void Stop();

	static void Update();

	static void SetLoop(bool value);

protected:
	bool m_isPlaying = false;
	bool m_loop = false;
	float m_volume = 1.f;

	const char* m_filePath = nullptr; // TODO : Make an audio file class instead

	ma_decoder m_decoder;
	ma_device m_device;
	ma_uint32 m_channelsCount = 0;

private:
	static void Data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

	void FlushCommandsImpl() override {};
	void BuildTasksImpl(TaskGraph& graph) override {};
	void OnInitialize() override {}
};

#endif
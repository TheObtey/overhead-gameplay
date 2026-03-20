#ifndef FOUNDRY_AUDIOENGINE__H_
#define FOUNDRY_AUDIOENGINE__H_

#define MINIAUDIO_IMPLEMENTATION //For miniaudio

#include "Define.h"
#include "miniaudio.h"

#include <iostream>

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();
	void Data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
	void PlayAudio(char filePath);

private:

};

#endif
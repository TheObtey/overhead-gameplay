#ifndef FOUNDRY_AUDIOENGINE__H_
#define FOUNDRY_AUDIOENGINE__H_

#include "Define.h"
#include "miniaudio.h"

#include <iostream>

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine() = default;
	
	void PlayAudio(const char* filePath);

protected:

private:
	static void Data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};

#endif
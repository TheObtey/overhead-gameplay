#define MINIAUDIO_IMPLEMENTATION

#include "Servers/AudioServer.h"

void AudioServer::Data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

void AudioServer::SetAudioFile(const char* filePath)
{
    Instance().m_filePath = filePath;
}

void AudioServer::Play()
{
    ma_result result;
    //ma_decoder decoder;
    ma_device_config deviceConfig;
    //ma_device device;

    result = ma_decoder_init_file(Instance().m_filePath, NULL, &Instance().m_decoder);
    if (result != MA_SUCCESS)
    {
        printf("Could not load file: %s\n", Instance().m_filePath);
        return;
    }

    if (Instance().m_loop) ma_data_source_set_looping(&Instance().m_decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = Instance().m_decoder.outputFormat;
    deviceConfig.playback.channels = Instance().m_decoder.outputChannels;
    deviceConfig.sampleRate = Instance().m_decoder.outputSampleRate;
    deviceConfig.dataCallback = Data_callback;
    deviceConfig.pUserData = &Instance().m_decoder;

    //ma_sound_set_volume();

    if (ma_device_init(NULL, &deviceConfig, &Instance().m_device) != MA_SUCCESS)
    {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&Instance().m_decoder);
        return;
    }

    if (ma_device_start(&Instance().m_device) != MA_SUCCESS)
    {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&Instance().m_device);
        ma_decoder_uninit(&Instance().m_decoder);
        return;
    }

    Instance().m_isPlaying = true;
}

void AudioServer::Stop()
{
    ma_device_uninit(&Instance().m_device);
    ma_decoder_uninit(&Instance().m_decoder);

    Instance().m_isPlaying = false;
}

void AudioServer::Update()
{
    if (Instance().m_isPlaying)
    {

    }
}

void AudioServer::SetLoop(bool value)
{
    Instance().m_loop = value;
}
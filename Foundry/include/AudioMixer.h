#ifndef MIXER_AUDIO__H_
#define MIXER_AUDIO__H_

#include "Servers/AudioServer.h"
#include "Define.h"

struct ExampleEntry
{
    ma_delay_node node;
    bool active = false;
};

struct DelayEntry
{
    ma_delay_node node;
    bool active = false;
};

struct ReverbEntry
{
    ma_node_base node;
    bool active = false;

    float wet = 0.3f;
    float roomSize = 0.85f;
    float comb[4][4096] = {};
    float ap[2][1024] = {};
    int ci[4] = {};
    int ai[2] = {};
};

// Input an audio to make different effects

class AudioMixer
{
public:
    void Shutdown();

    void AddDelay(AudioChannel* channel, float delaySeconds, float decay, float wet = 0.5f);
    void SetDelayWet(AudioChannel* channel, float wet);
    void RemoveDelay(AudioChannel* channel);

    void AddReverb(AudioChannel* channel, float roomSize, float wet);
    void SetReverbWet(AudioChannel* channel, float wet);
    void RemoveReverb(AudioChannel* channel);

protected:
    std::unordered_map<AudioChannel*, DelayEntry> m_delays;
    std::unordered_map<AudioChannel*, uptr<ReverbEntry>> m_reverbs;

private:
    static void ReverbProcess(ma_node* pNode, const float** in, uint32* inCount, float** out, uint32* outCount);

};

#endif
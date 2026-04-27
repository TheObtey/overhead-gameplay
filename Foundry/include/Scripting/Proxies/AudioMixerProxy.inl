class AudioMixer::Proxy
{
public:
    struct ProxyBinding;

    Proxy() = default;

    void AddDelay(AudioChannel* channel, float delaySeconds, float decay, float wet)
    {
        m_mixer.AddDelay(channel, delaySeconds, decay, wet);
    }
    void SetDelayWet(AudioChannel* channel, float wet)
    {
        m_mixer.SetDelayWet(channel, wet);
    }
    void RemoveDelay(AudioChannel* channel)
    {
        m_mixer.RemoveDelay(channel);
    }
    void AddReverb(AudioChannel* channel, float roomSize, float wet)
    {
        m_mixer.AddReverb(channel, roomSize, wet);
    }
    void SetReverbWet(AudioChannel* channel, float wet)
    {
        m_mixer.SetReverbWet(channel, wet);
    }
    void RemoveReverb(AudioChannel* channel)
    {
        m_mixer.RemoveReverb(channel);
    }
    void Shutdown()
    {
        m_mixer.Shutdown();
    }

private:
    AudioMixer m_mixer;
};

BindProxy(AudioMixer::Proxy,
    return binder.BindClass<AudioMixer::Proxy>("audiomixer",
        "AddDelay", BIND(AddDelay),
        "SetDelayWet", BIND(SetDelayWet),
        "RemoveDelay", BIND(RemoveDelay),
        "AddReverb", BIND(AddReverb),
        "SetReverbWet", BIND(SetReverbWet),
        "RemoveReverb", BIND(RemoveReverb),
        "Shutdown", BIND(Shutdown)
    );
)
REGISTER_PROXY(AudioMixer::Proxy::ProxyBinding, AudioMixerProxy);
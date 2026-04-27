#include "AudioMixer.h"
#include <algorithm>

static void ReverbProcess(ma_node* pNode, const float** in, uint32* inCount, float** out, uint32* outCount)
{
	for (ma_uint32 i = 0; i < *outCount; i++)
	{
		out[0][i * 2] = in[0][i * 2] * 0.1f;
		out[0][i * 2 + 1] = in[0][i * 2 + 1] * 0.1f;
	}
}

static ma_node_vtable g_reverbVtable = { ReverbProcess, NULL, 1, 1, 0 };

void AudioMixer::Shutdown()
{
	for (auto& [ch, entry] : m_delays)
	{
		if (entry.active)
		{
			ma_delay_node_uninit(&entry.node, NULL);
		}
	}

	for (auto& [ch, rev] : m_reverbs)
	{
		ma_node_uninit(&rev->node, NULL);
	}

	m_delays.clear();
	m_reverbs.clear();
}

void AudioMixer::AddDelay(AudioChannel* channel, float delaySeconds, float decay, float wet)
{
	if (!channel || m_delays.count(channel)) return;

	ma_engine& engine = AudioServer::GetSoundEngine();

	ma_uint32 sampleRate = ma_engine_get_sample_rate(&engine);
	ma_uint32 frames = (ma_uint32)(delaySeconds * sampleRate);

	ma_delay_node_config cfg = ma_delay_node_config_init(2, sampleRate, frames, decay);
	DelayEntry entry{};

	if (ma_delay_node_init(ma_engine_get_node_graph(&engine), &cfg, NULL, &entry.node) != MA_SUCCESS)
	{
		Logger::Log("[MixerAudio] AddDelay failed on " + channel->GetName());
		return;
	}

	ma_delay_node_set_wet(&entry.node, wet);
	ma_delay_node_set_dry(&entry.node, 1.0f);

	ma_node_attach_output_bus(&channel->GetGroup(), 0, &entry.node, 0);
	ma_node_attach_output_bus(&entry.node, 0, ma_engine_get_endpoint(&engine), 0);

	entry.active = true;
	m_delays[channel] = std::move(entry);
}

void AudioMixer::SetDelayWet(AudioChannel* channel, float wet)
{
	auto it = m_delays.find(channel);
	if (it == m_delays.end() || !it->second.active) return;
	ma_delay_node_set_wet(&it->second.node, std::clamp(wet, 0.0f, 1.0f));
}

void AudioMixer::RemoveDelay(AudioChannel* channel)
{
	auto it = m_delays.find(channel);
	if (it == m_delays.end()) return;
	if (it->second.active) ma_delay_node_uninit(&it->second.node, NULL);

	ma_node_attach_output_bus(&channel->GetGroup(), 0, ma_engine_get_endpoint(&AudioServer::GetSoundEngine()), 0);

	m_delays.erase(it);
}

void AudioMixer::AddReverb(AudioChannel* channel, float roomSize, float wet)
{
	if (!channel || m_reverbs.count(channel)) return;

	auto rev = std::make_unique<ReverbEntry>();
	rev->roomSize = std::clamp(roomSize, 0.0f, 0.98f);
	rev->wet = std::clamp(wet, 0.0f, 1.0f);
	rev->active = true;

	ma_engine& engine = AudioServer::GetSoundEngine();

	ma_uint32 inCh = 2;
	ma_uint32 outCh = 2;
	ma_node_config cfg = ma_node_config_init();
	cfg.vtable = &g_reverbVtable;
	cfg.pInputChannels = &inCh;
	cfg.pOutputChannels = &outCh;
	cfg.inputBusCount = 1;
	cfg.outputBusCount = 1;

	if (ma_node_init(ma_engine_get_node_graph(&engine), &cfg, NULL, &rev->node) != MA_SUCCESS)
	{
		Logger::Log("[MixerAudio] AddReverb failed on " + channel->GetName());
		return;
	}

	ma_node* groupNode = &channel->GetGroup().engineNode.baseNode;
	ma_node_attach_output_bus(groupNode, 0, &rev->node, 0);
	ma_node_attach_output_bus(&rev->node, 0, ma_engine_get_endpoint(&engine), 0);

	m_reverbs[channel] = std::move(rev);
}

void AudioMixer::SetReverbWet(AudioChannel* channel, float wet)
{
	auto it = m_reverbs.find(channel);
	if (it == m_reverbs.end()) return;
	it->second->wet = std::clamp(wet, 0.0f, 1.0f);
}

void AudioMixer::RemoveReverb(AudioChannel* channel)
{
	auto it = m_reverbs.find(channel);
	if (it == m_reverbs.end()) return;

	ma_node_uninit(&it->second->node, NULL);

	ma_node* groupNode = (ma_node*)&channel->GetGroup();
	ma_node_attach_output_bus(groupNode, 0, ma_engine_get_endpoint(&AudioServer::GetSoundEngine()), 0);

	m_reverbs.erase(it);
}
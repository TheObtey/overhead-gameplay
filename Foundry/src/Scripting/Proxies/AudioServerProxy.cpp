#include "Scripting/Proxies/AudioServerProxy.h"
#include "Servers/AudioServer.h"

AudioChannel* CreateChannel(const std::string& name)
{ 
	return AudioServer::CreateChannel(name);
}
void SetMasterVolume(float volume) 
{ 
	AudioServer::SetMasterVolume(volume);
}
float GetMasterVolume() 
{
	return AudioServer::GetMasterVolume();
}
void SetGroupVolume(AudioChannel* channel, float v)
{ 
	AudioServer::SetGroupVolume(channel, v);
}
float GetGroupVolume(AudioChannel* channel)
{ 
	return ma_sound_group_get_volume(&channel->GetGroup()); 
}
void AudioProxyBinding::Bind(Binder& binder)
{
	binder.BindClass<AudioChannel>("audiochannel", 
		"GetName",&AudioChannel::GetName,
		"GetGroup",&AudioChannel::GetGroup
		);

	auto audioNameSpace = binder.GetOrCreateNamespace("audio");
	audioNameSpace.set_function("CreateChannel", &CreateChannel);
	audioNameSpace.set_function("SetMasterVolume", &SetMasterVolume);
	audioNameSpace.set_function("GetMasterVolume", &GetMasterVolume);
	audioNameSpace.set_function("SetGroupVolume", &SetGroupVolume);
	audioNameSpace.set_function("GetGroupVolume", &GetGroupVolume);
}
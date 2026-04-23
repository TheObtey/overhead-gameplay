//#include "Scripting/Proxies/AudioServerProxy.h"
//#include "Registries/AutomaticRegisterProxy.hpp"
//#include "Servers/AudioServer.h"
//
//void AudioProxyBinding::Bind(Binder& binder)
//{
//	binder.BindClass<AudioChannel>("audiochannel", 
//		"GetName",&AudioChannel::GetName
//		);
//
//	auto audioNameSpace = binder.GetOrCreateNamespace("audioserver");
//
//	audioNameSpace.set_function("CreateChannel", &AudioServer::CreateChannel);
//	audioNameSpace.set_function("SetMasterVolume", &AudioServer::SetMasterVolume);
//	audioNameSpace.set_function("GetMasterVolume", &AudioServer::GetMasterVolume);
//	audioNameSpace.set_function("SetGroupVolume", &AudioServer::SetGroupVolume);
//	audioNameSpace.set_function("GetGroupVolume", &AudioServer::GetGroupVolume);
//}

//REGISTER_PROXY(AudioProxyBinding, AudioProxy);
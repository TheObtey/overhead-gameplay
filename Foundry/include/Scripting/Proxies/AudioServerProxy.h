#ifndef FOUNDRY_AUDIOPROXY__H_
#define FOUNDRY_AUDIOPROXY__H_

#include "Registries/AutomaticRegisterProxy.hpp"
#include "Servers/AudioServer.h"

//class Binder;

//struct AudioChannel;

struct AudioProxyBinding 
{
	static void Bind(Binder& binder)
	{
		binder.BindClass<AudioChannel>("audiochannel",
			"GetName", &AudioChannel::GetName
		);

		auto audioNameSpace = binder.GetOrCreateNamespace("audioserver");
		//audioNameSpace.new_usertype<AudioChannel>("audiochannel");

		audioNameSpace.set_function("CreateChannel", &AudioServer::CreateChannel);
		audioNameSpace.set_function("SetMasterVolume", &AudioServer::SetMasterVolume);
		audioNameSpace.set_function("GetMasterVolume", &AudioServer::GetMasterVolume);
		audioNameSpace.set_function("SetGroupVolume", &AudioServer::SetGroupVolume);
		audioNameSpace.set_function("GetGroupVolume", &AudioServer::GetGroupVolume);
	}
};


#endif
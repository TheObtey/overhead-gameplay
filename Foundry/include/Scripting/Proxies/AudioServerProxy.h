#ifndef FOUNDRY_AUDIOPROXY__H_
#define FOUNDRY_AUDIOPROXY__H_

class Binder;

struct AudioChannel;

//AudioChannel* CreateChannel(const std::string& name);
//void SetMasterVolume(float volume); 
//float GetMasterVolume();
//void SetGroupVolume(AudioChannel* channel, float v);
//float GetGroupVolume(AudioChannel* channel);

struct AudioProxyBinding 
{
	static void Bind(Binder& binder);
};

#endif
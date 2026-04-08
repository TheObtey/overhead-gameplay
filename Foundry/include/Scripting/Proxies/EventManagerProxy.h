#ifndef FOUNDRY_EVENTMANAGERPROXY__H_
#define FOUNDRY_EVENTMANAGERPROXY__H_

#include "Registries/AutomaticRegisterProxy.hpp"
#include "Scripting/Binder.h"

struct EventManagerProxyBinding
{
	static void Bind(Binder& binder);
};

REGISTER_PROXY(EventManagerProxyBinding, EventManagerProxy);

#endif
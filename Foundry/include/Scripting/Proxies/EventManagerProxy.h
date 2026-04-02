#ifndef FOUNDRY_EVENTMANAGERPROXY__H_
#define FOUNDRY_EVENTMANAGERPROXY__H_

#include "Event.hpp"
#include "EventManager.h"
#include "Registries/AutomaticRegisterProxy.hpp"
#include "Scripting/Binder.h"
#include "sol/sol.hpp"

#include <string>
#include <string_view>

struct EventManagerProxyBinding
{
	static void Bind(Binder& binder);
};

REGISTER_PROXY(EventManagerProxyBinding, EventManagerProxy);

#endif
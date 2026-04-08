#ifndef FOUNDRY_ACTIONMAPPROXY__H_
#define FOUNDRY_ACTIONMAPPROXY__H_

#include "IControl.h"
#include "Action.h"
#include "ActionMap.h"
#include "Event.hpp"
#include "Registries/AutomaticRegisterProxy.hpp"
#include "Scripting/Binder.h"
#include "sol/sol.hpp"

#include <string_view>
#include <unordered_map>
#include <string>


struct ActionMapProxyBinding
{
	static void Bind(Binder& binder);
};

REGISTER_PROXY(ActionMapProxyBinding, ActionMapProxy);

#endif
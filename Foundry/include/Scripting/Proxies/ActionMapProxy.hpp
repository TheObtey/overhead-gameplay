#ifndef FOUNDRY_ACTIONMAPPROXY__H_
#define FOUNDRY_ACTIONMAPPROXY__H_

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
	static void Bind(Binder& binder)
	{
		binder.BindClass<IControl>("icontrol",
			sol::constructors<IControl(ControlType const&, EventInput const&, Action*)>(),
			"GetControlType", &IControl::GetControlType,
			"GetEventInput", &IControl::GetEventInput,
			"Read", &IControl::Read,
			"SetAction", &IControl::SetAction
		);

		binder.BindClass<ButtonControl>("buttoncontrol",
			sol::base_classes, sol::bases<IControl>,
			sol::constructors<ButtonControl(EventInput const&, Action*)>(),
			"GetState", &ButtonControl::GetState
		);

		binder.BindClass<SliderControl>("slidercontrol",
			sol::base_classes, sol::bases<IControl>,
			sol::constructors<SliderControl(EventInput const&, Action*)>(),
			"GetPos", &SliderControl::GetPos
		);
		
		binder.BindClass<StickControl>("stickcontrol",
			sol::base_classes, sol::bases<IControl>,
			sol::constructors<StickControl(EventInput const&, Action*)>(),
			"GetPos", &StickControl::GetPos
		);

		
		binder.BindClass<Action>("action",
			sol::constructors<Action(ControlType, Event<void* (void*)>, EventInput)>(),
			"GetEvent", &Action::GetEvent,
			"AddControl", &Action::AddControl,
			"GetControl", &Action::GetControl
		);
		

		binder.BindClass<ActionMap>("actionmap",
			sol::constructors<ActionMap()>(),
			sol::meta_function::index, &ActionMap::operator[],
			"Emplace", &ActionMap::Emplace,
			"Erase", &ActionMap::Erase,
			"GetAction", &ActionMap::GetAction,
			"Length", &ActionMap::Length,
			"Rename", &ActionMap::Rename,
			"Active", &ActionMap::Active)
		);
	}
};

REGISTER_PROXY(ActionMapProxyBinding, ActionMapProxy);

#endif
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
	static void Bind(Binder& binder)
	{
		binder.BindEnum<ButtonState>("ButtonState",
			"Up", ButtonState::UP,
			"Down", ButtonState::DOWN
		);

		binder.BindEnum<ControlType>("ControlType",
			"Button", ControlType::BUTTON,
			"Slider", ControlType::SLIDER,
			"Stick", ControlType::STICK
		);


		binder.BindClass<IControl>("icontrol",
			sol::constructors<IControl(), IControl(ControlType const&, EventInput const&, Action*)>(),
			"GetControlType", &IControl::GetControlType,
			"GetEventInput", &IControl::GetEventInput,
			"ReadAsBool", &IControl::Read<bool>,
			"ReadAsFloat", &IControl::Read<float>,
			"ReadAsVec2", &IControl::Read<glm::vec2>,
			"SetAction", &IControl::SetAction
		);

		binder.BindClass<ButtonControl>("buttoncontrol",
			sol::constructors<ButtonControl(), ButtonControl(EventInput const&, Action*)>(),
			sol::base_classes, sol::bases<IControl>(),
			"GetState", &ButtonControl::GetState
		);

		binder.BindClass<SliderControl>("slidercontrol",
			sol::constructors<SliderControl(), SliderControl(EventInput const&, Action*)>(),
			sol::base_classes, sol::bases<IControl>(),
			"GetPos", &SliderControl::GetPos
		);
		
		binder.BindClass<StickControl>("stickcontrol",
			sol::constructors<StickControl(), StickControl(EventInput const&, Action*)>(),
			sol::base_classes, sol::bases<IControl>(),
			"GetPos", &StickControl::GetPos
		);
		
		
		
		binder.BindClass<Action>("action",
			sol::constructors<Action(), Action(ControlType, EventInput)>(),
			"GetEvent", &Action::Event,
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
			"Active", &ActionMap::Active
		);
	}
};

REGISTER_PROXY(ActionMapProxyBinding, ActionMapProxy);

#endif
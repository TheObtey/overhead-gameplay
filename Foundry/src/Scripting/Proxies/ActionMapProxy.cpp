#include "Scripting/Proxies/ActionMapProxy.h"
#include "Registries/AutomaticRegisterProxy.hpp"
#include "IControl.h"
#include "Action.h"
#include "ActionMap.h"
#include "Event.hpp"

#include <string_view>
#include <unordered_map>
#include <string>

void ActionMapProxyBinding::Bind(Binder& binder)
{
	binder.BindEnum<ButtonState>("ButtonState",
		"UP", ButtonState::UP,
		"DOWN", ButtonState::DOWN
	);

	binder.BindEnum<ControlType>("ControlType",
		"BUTTON", ControlType::BUTTON,
		"SLIDER", ControlType::SLIDER,
		"STICK", ControlType::STICK
	);


	binder.BindClass<IControl>("icontrol",
		sol::constructors<IControl(), IControl(ControlType const&, Ore::EventInput const&, Action*)>(),
		"GetControlType", &IControl::GetControlType,
		"GetEventInput", &IControl::GetEventInput,
		"ReadAsBool", &IControl::Read<bool>,
		"ReadAsFloat", &IControl::Read<float>,
		"ReadAsVec2", &IControl::Read<glm::vec2>
	);

	binder.BindClass<ButtonControl>("buttoncontrol",
		sol::constructors<ButtonControl(), ButtonControl(Ore::EventInput const&, Action*)>(),
		sol::base_classes, sol::bases<IControl>(),
		"GetState", &ButtonControl::GetState
	);

	binder.BindClass<SliderControl>("slidercontrol",
		sol::constructors<SliderControl(), SliderControl(Ore::EventInput const&, Action*)>(),
		sol::base_classes, sol::bases<IControl>(),
		"GetPos", &SliderControl::GetPos
	);

	binder.BindClass<StickControl>("stickcontrol",
		sol::constructors<StickControl(), StickControl(Ore::EventInput const&, Action*)>(),
		sol::base_classes, sol::bases<IControl>(),
		"GetPos", &StickControl::GetPos
	);



	binder.BindClass<Action>("action",
		"Event", &Action::Event,
		"AddControl", &Action::AddControl,
		"GetControl", &Action::GetControl
	);


	binder.BindClass<ActionMap>("actionmap",
		sol::constructors<ActionMap(std::string const&)>(),
		sol::meta_function::index, &ActionMap::operator[],
		"CreateAction", &ActionMap::CreateAction,
		"Erase", &ActionMap::Erase,
		"GetAction", &ActionMap::GetAction,
		"Length", &ActionMap::Length,
		"Rename", &ActionMap::Rename,
		"SetCurrentActionMap", &ActionMap::SetCurrentActionMap
	);
}

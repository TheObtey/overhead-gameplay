#include "Scripting/Proxies/ActionMapProxy.h"

void ActionMapProxyBinding::Bind(Binder& binder)	
{
	binder.BindEnum<ButtonState>("buttonstate",
		"Up", ButtonState::UP,
		"Down", ButtonState::DOWN
	);

	binder.BindEnum<ControlType>("controltype",
		"Button", ControlType::BUTTON,
		"Slider", ControlType::SLIDER,
		"Stick", ControlType::STICK
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
		"Emplace", &ActionMap::Emplace,
		"Erase", &ActionMap::Erase,
		"GetAction", &ActionMap::GetAction,
		"Length", &ActionMap::Length,
		"Rename", &ActionMap::Rename,
		"CreateAction", &ActionMap::CreateAction,
		"Active", &ActionMap::Active
	);
	}
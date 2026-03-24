#include "IControl.h"

#include "glm/glm.hpp"

IControl::IControl(ControlType const& type, EventInput const& eventInput) : m_type(type), m_eventInput(eventInput), m_action(nullptr) {}

IControl::~IControl()
{
	m_action = nullptr;
}

ControlType IControl::GetControlType() const
{
	return m_type;
}


ButtonControl::ButtonControl(EventInput const& eventInput) : IControl(ControlType::BUTTON, eventInput), m_state(ButtonState::UP) {}

ButtonControl::ButtonState ButtonControl::GetState()
{
	return m_state;
}

template <>
bool IControl::Read<bool>(IControl& iControl)
{
	if (iControl.m_type != ControlType::BUTTON)
		return false;

	return (bool)static_cast<ButtonControl&>(iControl).GetState();
}

template <>
float IControl::Read<float>(IControl& iControl)
{
	if (iControl.m_type != ControlType::SLIDER)
		return 0.0f;

	return static_cast<SliderControl&>(iControl).GetPos();
}

template <>
glm::vec2 IControl::Read<glm::vec2>(IControl& iControl)
{
	if (iControl.m_type != ControlType::STICK)
		return { 0.0f, 0.0f };

	return static_cast<StickControl&>(iControl).GetPos();
}


SliderControl::SliderControl(EventInput const& eventInput) : IControl(ControlType::SLIDER, eventInput), m_pos(0.0f) {}

float SliderControl::GetPos() const
{
	return m_pos;
}

StickControl::StickControl(EventInput const& eventInput) : IControl(ControlType::SLIDER, eventInput), m_pos(0.0f) {}

bool StickControl::IsFlicked() const
{
	if (m_pos == glm::vec2(0.0f, 0.0f))
		return false;

	return true;
}

glm::vec2 StickControl::GetPos() const
{
	return m_pos;
}

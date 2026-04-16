#include "IControl.h"

#include "glm/glm.hpp"

IControl::IControl(ControlType const& type, Ore::EventInput const& eventInput, Action* pAction) :
	m_type(type), m_eventInput(eventInput), m_pAction(pAction) {}

IControl::~IControl()
{
	m_pAction = nullptr;
}

ControlType IControl::GetControlType() const
{
	return m_type;
}

Ore::EventInput IControl::GetEventInput() const
{
	return m_eventInput;
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

void IControl::SetAction(Action* pAction)
{
	m_pAction = pAction;
}

bool IControl::IsPressed()
{
	if (m_type != ControlType::BUTTON) return false;

	ButtonControl* btn = static_cast<ButtonControl*>(this);
	return btn->GetState() == ButtonState::PRESSED;
}

bool IControl::IsReleased()
{
	if (m_type != ControlType::BUTTON) return false;

	ButtonControl* btn = static_cast<ButtonControl*>(this);
	return btn->GetState() == ButtonState::RELEASED;
}

bool IControl::IsHold()
{
	if (m_type != ControlType::BUTTON) return false;

	ButtonControl* btn = static_cast<ButtonControl*>(this);
	return btn->GetState() == ButtonState::HOLD;
}

ButtonControl::ButtonControl(Ore::EventInput const& eventInput, Action* pAction) : IControl(ControlType::BUTTON, eventInput, pAction), m_state(ButtonState::NONE) {}

ButtonState ButtonControl::GetState() const
{
	return m_state;
}

void ButtonControl::SetState(ButtonState state)
{
	m_state = state;
}


SliderControl::SliderControl(Ore::EventInput const& eventInput, Action* pAction) : IControl(ControlType::SLIDER, eventInput, pAction), m_pos(0.0f) {}

float SliderControl::GetPos() const
{
	return m_pos;
}

void SliderControl::SetPos(float pos)
{
	m_pos = pos;
}


StickControl::StickControl(Ore::EventInput const& eventInput, Action* pAction) : IControl(ControlType::STICK, eventInput, pAction), m_pos(0.0f) {}

glm::vec2 StickControl::GetPos() const
{
	return m_pos;
}

void StickControl::SetPos(glm::vec2 pos)
{

	m_pos = pos;
}

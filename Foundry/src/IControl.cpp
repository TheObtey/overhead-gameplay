#include "IControl.h"

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

void ButtonControl::Read(IControl& iControl)
{
	//
}


SliderControl::SliderControl(EventInput const& eventInput) : IControl(ControlType::SLIDER, eventInput), m_pos(0.0f) {}

float SliderControl::GetPos() const
{
	return m_pos;
}

void SliderControl::Read(IControl& iControl)
{

	//

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

void StickControl::Read(IControl& iControl)
{

	//

}
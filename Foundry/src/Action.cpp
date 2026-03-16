#include "IAction.h"

Action::Action(std::string name) : m_name(name), m_controls() {}

template <typename RV, typename... Args>
Action::Action(std::string name, ControlType type, Event<RV, Args...> event, EventInput eventInput) : m_name(name), m_controls(), m_eventIsSet(false)
{
	AddControl(eventInput);
	SetEvent(event);
}

template <typename RV, typename... Args>
void Action::SetEvent(Event<RV(Args...)> const& event)
{
	for (int i = 0; i < m_controls.size(); i++)
	{
		switch (m_controls[i]->m_type)
		{
		case ControlType::BUTTON:
			static_cast<SliderControl>(m_controls[i]).SetEvent(event);
			break;
		case ControlType::SLIDER:
			static_cast<ButtonControl>(m_controls[i]).SetEvent(event);
			break;
		case ControlType::STICK:
			static_cast<StickControl>(m_controls[i]).SetEvent(event);
			break;
		}
	}
}

template <typename RV, typename... Args>
Event<RV(Args...)>& Action::GetEvent(uint32 index) const
{
	return m_event;
}

uint32 Action::AddControl(ControlType const& type, EventInput const& eventInput)
{
	switch (type)
	{
	case ControlType::BUTTON:
		m_controls.push_back(new ButtonControl(eventInput));
		break;
	case ControlType::SLIDER:
		m_controls.push_back(new SliderControl(eventInput));
		break;
	case ControlType::STICK:
		m_controls.push_back(new StickControl(eventInput));
		break;
	}
}

IControl::IControl(ControlType const& type, EventInput const& eventInput) : m_type(type), m_eventInput(eventInput), m_action(nullptr) {}

ControlType IControl::GetControlType() const
{
	return m_type;
}


ButtonControl::ButtonControl(EventInput const& eventInput) : IControl(ControlType::BUTTON, eventInput), m_state(ButtonState::UP) {}

ButtonControl::~ButtonControl()
{
	m_action = nullptr;
}

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





#include "Action.h"
#include "IControl.h"

Action::Action(std::string name) : m_name(name), m_controls() {}

template <typename RV, typename... Args>
Action::Action(std::string name, ControlType type, Event<RV, Args...> event, EventInput eventInput) : m_name(name), m_controls()
{
	AddControl(eventInput);
	
	switch (type)
	{
	case ControlType::BUTTON:
		SetEvent<void, bool>(event);
		break;
	case ControlType::SLIDER:
		SetEvent<void, float>(event);
		break;
	case ControlType::STICK:
		SetEvent<void, glm::vec2>(event);
		break;
	}
}

template <typename RV, typename... Args>
void Action::SetEvent(Event<RV(Args...)> const& event)
{
	for (int i = 0; i < m_controls.size(); i++)
	{
		//
	}
}

Event<void(IControl&)> Action::GetEvent() const
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
	return m_controls.size() - 1;
}

std::vector<IControl*>& Action::GetControls()
{
	return m_controls;
}





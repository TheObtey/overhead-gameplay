#include "Action.h"
#include "IControl.h"

Action::Action(std::string name) : m_controls(), m_event() {}

template <typename RV, typename... Args>
Action::Action(std::string name, ControlType type, Event<RV, Args...> event, EventInput eventInput) :
	m_name(name), m_event(event)
{
	AddControl(type, eventInput);
}

template <typename RV, typename... Args>
void Action::SetEvent(Event<RV(Args...)> const& event)
{
	m_event = event;
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
		m_controls.push_back(new ButtonControl(eventInput, this));
		break;
	case ControlType::SLIDER:
		m_controls.push_back(new SliderControl(eventInput, this));
		break;
	case ControlType::STICK:
		m_controls.push_back(new StickControl(eventInput, this));
		break;
	}

	m_controls.back()->SetAction(this);

	return m_controls.size() - 1;
}

IControl* Action::GetControl(uint32& index)
{
	if (index > m_controls.size())
		return nullptr;

	return m_controls[index];
}

void Action::SetName(std::string const& name)
{
	m_name = name;
}

std::string_view Action::GetName() const
{
	return m_name;
}

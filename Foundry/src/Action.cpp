#include "Action.h"
#include "IControl.h"

Action::Action() : m_controls(), OnAction()
{
	Ore::EventManager::getKey += [&](Ore::EventInput in, Ore::EventAction ac)
		{
			for (int i = 0; i < m_controls.size(); i++)
			{
				if (in == m_controls[i]->GetEventInput())
					OnAction.Invoke(*m_controls[i]);
			}
		};
}

Action::Action(ControlType controlType, Ore::EventInput eventInput) :
	m_controls(), OnAction()
{
	AddControl(controlType, eventInput);
	Ore::EventManager::getKey += [&](Ore::EventInput in, Ore::EventAction ac)
		{
			for (int i = 0; i < m_controls.size(); i++)
			{
				if (in == m_controls[i]->GetEventInput() && ac == Ore::EventAction::PRESS)
					OnAction.Invoke(*m_controls[i]);
			}
		};
}

Action::~Action() {}


uint32 Action::AddControl(ControlType const& type, Ore::EventInput const& eventInput)
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

IControl* Action::GetControl(uint32 index)
{
	if (index > m_controls.size())
		return nullptr;

	return m_controls[index];
}

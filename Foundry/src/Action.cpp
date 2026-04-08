#include "Action.h"
#include "EventManager.h"
#include "ActionMap.h"

Action::Action(ControlType controlType, EventInput eventInput, ActionMap* pActionMap) :
	m_controls(), Event(), m_pOwner(pActionMap)
{
	AddControl(controlType, eventInput);
	EventManager::getKey += [&](EventInput in, EventAction ac)
		{
			if (m_pOwner == nullptr || m_pOwner->Active == false)
				return;
			for (int i = 0; i  < m_controls.size(); i++)
			{
				if (in == m_controls[i].GetEventInput() && ac == EventAction::PRESS)
					std::invoke(Event, m_controls[i]);
			}
		};
}

Action::~Action() {}


uint32 Action::AddControl(ControlType const& type, EventInput const& eventInput)
{
	switch (type)
	{
	case ControlType::BUTTON:
		m_controls.push_back(ButtonControl(eventInput, this));
		break;
	case ControlType::SLIDER:
		m_controls.push_back(SliderControl(eventInput, this));
		break;
	case ControlType::STICK:
		m_controls.push_back(StickControl(eventInput, this));
		break;
	}

	return m_controls.size() - 1;
}

IControl& Action::GetControl(uint32 index)
{
	IControl ic = IControl();
	if (index > m_controls.size())
		return ic;
	return m_controls[index];
}

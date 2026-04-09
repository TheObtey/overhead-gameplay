#include "Action.h"
#include "EventManager.h"
#include "ActionMap.h"

Action::Action(ControlType controlType, Ore::EventInput eventInput, ActionMap* pActionMap) :
	m_controls(std::vector<IControl*>()), Event(), m_pOwner(pActionMap)
{
	AddControl(controlType, eventInput);

	if (controlType == ControlType::STICK && eventInput == Ore::EventInput::MOUSE_MOVE)
	{
		Ore::EventManager::getCursorPos += [&](int32 x, int32 y)
			{
				if (m_pOwner == nullptr || m_pOwner->Active == false)
					return;
				for (int i = 0; i < m_controls.size(); i++)
				{
					if (m_controls[i]->GetEventInput() == Ore::EventInput::MOUSE_MOVE)
						std::invoke(Event, *m_controls[i]);
				}
			};
	}

	Ore::EventManager::getKey += [&](Ore::EventInput in, Ore::EventAction ac)
		{
			if (m_pOwner == nullptr || m_pOwner->Active == false)
				return;
			for (int i = 0; i  < m_controls.size(); i++)
			{
				if (in == m_controls[i]->GetEventInput() && ac == Ore::EventAction::PRESS)
 					std::invoke(Event, *m_controls[i]);
			}
		};
}

Action::~Action() 
{
	for (IControl* iControl : m_controls)
		delete iControl;

	m_pOwner = nullptr;
}


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

	return m_controls.size() - 1;
}

IControl& Action::GetControl(uint32 index)
{
	IControl ic = IControl();
	if (index > m_controls.size())
		return ic;
	return *m_controls[index];
}

#include "Action.h"
#include "EventManager.h"
#include "ActionMap.h"
#include "Debug.h"
#include "GameLoop.h"

Action::Action(ControlType controlType, Ore::EventInput eventInput, ActionMap* pActionMap) :
	m_controls(std::vector<IControl*>()), Event(), m_pOwner(pActionMap)
{
	AddControl(controlType, eventInput);

	Ore::EventManager::getCursorPos += [&](float const newX, float const newY){ HandleCursorMove(newX, newY); };
	Ore::EventManager::getKey += [&](Ore::EventInput const in, Ore::EventAction const ac) { HandleKeyDown(in, ac); };
}

void Action::HandleCursorMove(float newX, float newY)
{
	if (m_pOwner == nullptr || (GameLoop::CurrentActionMap != m_pOwner))
		return;

	for (int i = 0; i < m_controls.size(); i++)
	{
		if (m_controls[i]->GetEventInput() == Ore::EventInput::MOUSE_MOVE &&
			m_controls[i]->GetControlType() == ControlType::STICK)
		{
			StickControl& stick = static_cast<StickControl&>(*m_controls[i]);
			stick.SetPos({ newX, newY });
			std::invoke(Event, *m_controls[i]);
		}
	}
}

void Action::HandleKeyDown(Ore::EventInput const in, Ore::EventAction const ac)
{
	if (m_pOwner == nullptr || (GameLoop::CurrentActionMap != m_pOwner))
		return;

	for (int i = 0; i  < m_controls.size(); i++)
	{
		if (in == m_controls[i]->GetEventInput() && m_controls[i]->GetControlType() == ControlType::BUTTON)
		{
			ButtonControl& button = static_cast<ButtonControl&>(*m_controls[i]);
			if (ac == Ore::EventAction::PRESS)
				button.SetState(ButtonState::PRESSED);
			else if (ac == Ore::EventAction::RELEASE)
				button.SetState(ButtonState::RELEASED);
		}
	}
};

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
	case ControlType::UNDEFINED:
		break;
	}

	return m_controls.size() - 1;
}

IControl& Action::GetControl(uint32 const index)
{
	ASSERTM(index < m_controls.size(), "Index does not exist");
	return *m_controls[index];
}

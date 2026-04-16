#include "ActionMap.h"
#include "Action.h"
#include "GameLoop.h"

#include <functional>

ActionMap::ActionMap(std::string const& name) : m_name(name), m_actions(std::unordered_map<std::string, Action*>()) 
{
	GameLoop::CurrentActionMap = this;
}

ActionMap::~ActionMap()
{
	for (auto it = m_actions.begin(); it != m_actions.end(); it++)
		delete it->second;
}


Action* ActionMap::CreateAction(std::string_view const& name, ControlType type, Ore::EventInput eventInput)
{
	if (m_actions.find(std::string(name)) != m_actions.end())
		return nullptr;

	Action* pAction = new Action(type, eventInput, this);

	m_actions[std::string(name)] = pAction;

	return pAction;
}

bool ActionMap::Erase(std::string_view const& name)
{
	auto it = m_actions.find(std::string(name));

	if (it == m_actions.end())
		return false;
	
	m_actions.erase(it);

	return true;
}

Action* ActionMap::GetAction(std::string_view const& name)
{
	if (m_actions.find(std::string(name)) == m_actions.end())
		return nullptr;
	return m_actions[std::string(name)];
}

Action* ActionMap::operator[](std::string const& name)
{
	return GetAction(name);
}

uint32 ActionMap::Length() const
{
	return m_actions.size();
}

void ActionMap::Rename(std::string_view const& old, std::string_view const& name)
{
	if (m_actions.find(std::string(old)) == m_actions.end())
		return;

	m_actions[std::string(name)] = m_actions[std::string(old)];
	m_actions.erase(m_actions.find(std::string(old)));
}

void ActionMap::PollInputs(ActionMap* actionMap)
{
	if (actionMap != GameLoop::CurrentActionMap)
		return;

	for (auto it = actionMap->m_actions.begin(); it != actionMap->m_actions.end(); ++it)
	{
		auto& controls = it->second->m_controls;
		for (int i = 0; i < controls.size(); i++)
		{
			if (controls[i]->GetControlType() != ControlType::BUTTON)
				continue;

			ButtonControl& button = static_cast<ButtonControl&>(*controls[i]);

			if (button.GetState() == ButtonState::NONE)
				continue;

			std::invoke(it->second->Event, *controls[i]);

			if (button.GetState() == ButtonState::RELEASED)
				button.SetState(ButtonState::NONE);

			if (button.GetState() == ButtonState::PRESSED || button.GetState() == ButtonState::HOLD)
				button.SetState(ButtonState::HOLD);
		}
	}
}

void ActionMap::SetCurrentActionMap(ActionMap* actionMap)
{
	GameLoop::CurrentActionMap = actionMap;
}
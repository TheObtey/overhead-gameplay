#include "ActionMap.h"
#include "Action.h"
#include "Event.hpp"

ActionMap::ActionMap() : m_actions(std::unordered_map<std::string, std::vector<Action*>>()) {}

bool ActionMap::AddAction(std::string_view const& name, Action* pAction)
{
	if (pAction == nullptr)
		return false;

	m_actions[std::string(name)].push_back(pAction);
	return true;
}

bool ActionMap::EraseLastAction(std::string_view const& name)
{
	if (m_actions.find(std::string(name)) == m_actions.end())
		return false;
	m_actions[std::string(name)].erase(m_actions[std::string(name)].end());
	return true;
}

bool ActionMap::EraseAction(std::string_view const& name, uint32 const& index)
{
	auto it = m_actions.find(std::string(name));

	if (it == m_actions.end())
		return false;

	if (index > (*it).second.size())
		return false;

	(*it).second.erase((*it).second.begin() + index);
	return true;
}

template <typename RV, typename... Args>
bool ActionMap::SetAction(std::string_view const& name, Event<RV(Args...)>* pAction)
{
	if (m_actions.find(std::string(name)) == m_actions.end())
		return false;

	std::vector<Action*>& vector = m_actions[std::string(name)];

	for (int i = 0; i < vector.size(); i++)
	{
		vector[i]->SetEvent(pAction);
	}
	return true;
}

void ActionMap::ExecuteAction(std::string_view const& name)
{
	if (m_actions.find(std::string(name)) == m_actions.end())
		return;

	std::vector<Action*>& vector = m_actions[std::string(name)];

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = 0; j < vector[i]->m_controls.size(); j++)
		{
			vector[i]->GetEvent().Invoke(vector[j]->GetControls())
		}


	}
	
}



#include "ActionMap.h"
#include "Action.h"

ActionMap::ActionMap() : m_actions(std::unordered_map<std::string, Action*>()) {}

ActionMap::~ActionMap() {}

bool ActionMap::Emplace(std::string_view const& name, Action* pAction)
{
	if (pAction == nullptr)
		return false;

	if (m_actions.find(std::string(name)) != m_actions.end())
		return false;

	m_actions[std::string(name)] = pAction;
	
	return true;
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
	if (m_actions.find(std::string(name)) == m_actions.end())
		return nullptr;

	return m_actions[std::string(name)];
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

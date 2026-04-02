#ifndef FOUNDRY_ACTIONMAP_H__
#define FOUNDRY_ACTIONMAP_H__


#include "Event.hpp"
#include "IControl.h"


#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>


class Action;


/// <summary>
/// ActionMap is a custom container made to store Action objects, with each Action representing a Callback specified to a group of inputs
/// </summary>
class ActionMap
{
public:
	ActionMap();
	~ActionMap();

	ActionMap(ActionMap const& other) = delete;
	void operator=(ActionMap const& other) = delete;

	ActionMap(ActionMap&& other) = delete;
	void operator=(ActionMap&& other) = delete;


	bool	Emplace(std::string_view const& name, Action* pAction);
	bool	Erase(std::string_view const& name);
			
	Action*	GetAction(std::string_view const& name);
	Action*	operator[](std::string const& name);
			
	uint32	Length() const;
	void	Rename(std::string_view const& old, std::string_view const& name);

	bool	Active = true;

protected:
	std::unordered_map<std::string, Action*> m_actions;

	friend struct ActionMapProxyBinding;
};


#endif
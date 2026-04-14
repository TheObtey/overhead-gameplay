#ifndef FOUNDRY_ACTIONMAP_H__
#define FOUNDRY_ACTIONMAP_H__


#include "IControl.h"


#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>


class Action;


/// <summary>
/// ActionMap is a custom container made to store Action objects, with each Action detaining a Callback made to be called by a group of inputs
/// </summary>
class ActionMap
{
public:
	ActionMap(std::string const& name);
	~ActionMap();

	ActionMap(ActionMap const& other) = delete;
	void operator=(ActionMap const& other) = delete;

	ActionMap(ActionMap&& other) = delete;
	void operator=(ActionMap&& other) = delete;


	Action*	CreateAction(std::string_view const& name, ControlType type, Ore::EventInput eventInput);
	bool	Erase(std::string_view const& name);

	Action*	GetAction(std::string_view const& name);
	Action*	operator[](std::string const& name);

	uint32	Length() const;
	void	Rename(std::string_view const& old, std::string_view const& name);


	static void PollInputs(ActionMap* actionMap);
	static void SetCurrentActionMap(ActionMap* actionMap);

protected:
	std::string_view m_name;
	std::unordered_map<std::string, Action*> m_actions;

	friend struct ActionMapProxyBinding;
};


#endif
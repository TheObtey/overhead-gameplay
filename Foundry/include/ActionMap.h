#ifndef FOUNDRY_ACTIONMAP_H__
#define FOUNDRY_ACTIONMAP_H__


#include "Event.hpp"
#include "IControl.h"


#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>


class Action;


class ActionMap /*: public ISerializable*/
{
public:
	ActionMap();
	~ActionMap();

	ActionMap(ActionMap const& other) = delete;
	void operator=(ActionMap const& other) = delete;

	ActionMap(ActionMap&& other) = delete;
	void operator=(ActionMap&& other) = delete;


	bool		Emplace(std::string_view const& name, Action* pAction);
	void		Erase(std::string_view const& name);
				
	template	<typename RV, typename... Args>
	bool		SetAction(std::string_view const& name, Event<RV(Args...)>* pAction);
	Action*		GetAction(std::string_view const& name);
	Action*		operator[](std::string const& name);
				
	uint32		Length() const;
	void		Rename(std::string_view const& old, std::string_view const& name);

private:
	std::unordered_map<std::string, Action*> m_actions;
};


#endif
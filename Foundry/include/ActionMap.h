#ifndef FOUNDRY_ACTIONMAP__H_
#define FOUNDRY_ACTIONMAP__H_

#include "Action.h"
#include "IControl.h"

#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>


class ActionMap /*: public ISerializable*/
{
public:
	ActionMap() : m_actions() {};
	~ActionMap() = default;

	ActionMap(ActionMap const& other) = delete;
	void operator=(ActionMap const& other) = delete;

	ActionMap(ActionMap&& other) = delete;
	void operator=(ActionMap&& other) = delete;


	bool			AddAction(std::string_view const& name, Action* input);
	bool			DeleteAction(std::string_view const& name);

	template <typename RV, typename... Args>
	bool			SetAction(std::string_view const& name, Event<RV(Args...)>* pAction);
	void			ExecuteAction(std::string_view const& name) const;
	
	ButtonControl&  GetActionButton(std::string_view const& name) const;
	SliderControl&  GetActionSlider(std::string_view const& name) const;
	StickControl&   GetActionStick(std::string_view const& name) const;
	

	int64			Length() const { return m_actions.size(); }
	void			Pop(std::string_view const& name);
	void			Replace(std::string_view const& old, std::string_view const& name);

private:
	std::unordered_map<std::string, std::vector<Action*>> m_actions;
};


#endif
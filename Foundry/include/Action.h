#ifndef FOUNDRY_ACTION_H__
#define FOUNDRY_ACTION_H__


#include "Define.h"
#include "Serialization/ISerializable.h"
#include "Event.hpp"


#include <glm/glm.hpp>
#include <vector>


class IControl;
class ActionMap;
enum class EventInput;
enum class ControlType : byte;


class Action /*: public ISerializable*/
{
public:
	Action(std::string name = "DEFAULT_ACTION_NAME");
	
	template <typename RV, typename... Args>
	Action(
		std::string name, 
		ControlType type, 
		Event<RV, Args...> event,
		EventInput eventInput
	);

	~Action();
	
	template <typename RV, typename... Args>
	void SetEvent(Event<RV(Args...)> const& event);
	Event<void(IControl&)> GetEvent() const;

	uint32 AddControl(ControlType const& type, EventInput const& eventInput);
	IControl* GetControl(uint32& index);

	void SetName(std::string const& name);
	std::string_view GetName() const;

private:
	std::string_view m_name;
	std::vector<IControl*> m_controls; 
	Event<void(IControl&)> m_event;

	friend ActionMap;
};


#endif
#ifndef FOUNDRY_ACTION__H_
#define FOUNDRY_ACTION__H_

#include "Define.h"
#include "Serialization/ISerializable.h"
#include "Event.hpp"

#include <glm/glm.hpp>
#include <vector>

class IControl;
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
		EventInput eventInput = NULL
	);

	virtual ~Action();

	
	template <typename RV, typename... Args>
	void SetEvent(Event<RV(Args...)> const& event);

	template <typename RV, typename... Args>
	Event<RV(Args...)>& GetEvent() const;

	uint32 AddControl(ControlType const& type, EventInput const& eventInput);

	//bool RemoveControl(uint32 const& index);

private:
	std::string m_name;
	std::vector<IControl*> m_controls; 

	Event<void(IControl&)> m_event;
};

#endif
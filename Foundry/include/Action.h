#ifndef FOUNDRY_ACTION__H_
#define FOUNDRY_ACTION__H_

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
		EventInput eventInput = NULL
	);

	virtual ~Action();

	
	template <typename RV, typename... Args>
	void SetEvent(Event<RV(Args...)> const& event);

	Event<void(IControl&)> GetEvent() const;

	uint32 AddControl(ControlType const& type, EventInput const& eventInput);

	//bool RemoveControl(uint32 const& index);

protected:
	std::vector<IControl*>& GetControls();

private:
	std::string m_name;
	std::vector<IControl*> m_controls; 

	Event<void(IControl&)> m_event;

	friend ActionMap;
};

#endif
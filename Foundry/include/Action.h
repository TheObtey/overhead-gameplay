#ifndef FOUNDRY_ACTION_H__
#define FOUNDRY_ACTION_H__


#include "Define.h"
#include "Serialization/ISerializable.h"
#include "Event.hpp"
#include "EventManager.h"
#include "IControl.h"


#include <glm/glm.hpp>
#include <vector>


class ActionMap;
enum class EventInput;
enum class ControlType : byte;


class Action /*: public ISerializable*/
{
public:
	Action();
	
	template <typename RV, typename... Args>
	Action(
		ControlType type, 
		Event<RV, Args...> event,
		EventInput eventInput
	) : m_event(event)
	{
		AddControl(type, eventInput);
		EventManager::getKey += [&](EventInput in, EventAction ac)
			{
				for (int i = 0; i < m_controls.size(); i++)
				{
					if (in == m_controls[i]->GetEventInput())
						m_event.Invoke(*m_controls[i]);
				}
			};
	}

	~Action();
	
	Event<void(IControl&)> GetEvent() const;

	uint32 AddControl(ControlType const& type, EventInput const& eventInput);
	IControl* GetControl(uint32 index);

private:
	std::vector<IControl*> m_controls; 
	Event<void(IControl&)> m_event;

	friend ActionMap;
};


#endif
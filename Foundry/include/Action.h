#ifndef FOUNDRY_ACTION_H__
#define FOUNDRY_ACTION_H__

#include "Define.h"
#include "Event.hpp"
#include "EventManager.h"
#include "IControl.h"

#include <vector>

class ActionMap;
enum class EventInput;
enum class ControlType : byte;


class Action /*: public ISerializable*/
{
public:
	Action();
	Action(ControlType controlType, EventInput eventInput);

	~Action();

	uint32 AddControl(ControlType const& type, EventInput const& eventInput);
	IControl* GetControl(uint32 index);

	Event<void(IControl&)> OnAction;

private:
	std::vector<IControl*> m_controls;

	friend ActionMap;
};


#endif
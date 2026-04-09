#ifndef FOUNDRY_ACTION_H__
#define FOUNDRY_ACTION_H__


//#include "Serialization/ISerializable.h"
#include "IControl.h"


#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <functional>


class ActionMap;
enum class ControlType : byte;


class Action /*: public ISerializable*/
{
public:
	Action(ControlType controlType, Ore::EventInput eventInput, ActionMap* pActionMap = nullptr);
	~Action();

	uint32 AddControl(ControlType const& type, Ore::EventInput const& eventInput);
	IControl& GetControl(uint32 index);

	std::function<void(IControl&)> Event;

private:
	std::vector<IControl*> m_controls;
	ActionMap* m_pOwner;

	friend ActionMap;
};


#endif
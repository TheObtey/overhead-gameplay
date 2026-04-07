#ifndef FOUNDRY_ICONTROL_H__
#define FOUNDRY_ICONTROL_H__


#include "Define.h"
#include "EventManager.h"


#include <glm/glm.hpp>


class Action;
enum class EventInput;


enum class ControlType : byte
{
	BUTTON = 1,
	SLIDER = 2,
	STICK = 3,

	UNDEFINED = 0
};

enum class ButtonState : bool
{
	UP = false,
	DOWN = true
};


class IControl /*: public ISerializable*/
{
public:
	IControl() = default;
	IControl(ControlType const& type, EventInput const& eventInput, Action* pAction = nullptr);
	virtual ~IControl();

	ControlType GetControlType() const;
	EventInput GetEventInput() const;
	
	template <typename T>
	static T Read(IControl& iControl);

	void SetAction(Action* pAction);

private:
	ControlType m_type;
	EventInput m_eventInput;
	Action* m_pAction; 
};

class ButtonControl : public IControl
{
public:
	ButtonControl() = default;
	ButtonControl(EventInput const& eventInput, Action* pAction);
	~ButtonControl() = default;

	ButtonState GetState() const;

private:
	ButtonState m_state;
};


class SliderControl : public IControl
{
public:
	SliderControl() = default;
	SliderControl(EventInput const& eventInput, Action* pAction);
	~SliderControl() = default;

	float GetPos() const;

private:
	float m_pos;
};


class StickControl : public IControl
{
public:
	StickControl() = default;
	StickControl(EventInput const& eventInput, Action* pAction);
	~StickControl() = default;

	glm::vec2 GetPos() const;

private:
	glm::vec2 m_pos;
};

#endif
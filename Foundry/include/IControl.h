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
	IControl(ControlType const& type, EventInput const& eventInput, Action* pAction = nullptr);
	virtual ~IControl();

	ControlType GetControlType() const;
	
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
	ButtonControl(EventInput const& eventInput, Action* pAction);
	~ButtonControl() = default;

	ButtonState GetState();

private:
	ButtonState m_state;
};


class SliderControl : public IControl
{
public:
	SliderControl(EventInput const& eventInput, Action* pAction);
	~SliderControl() = default;

	float GetPos() const;

private:
	float m_pos;
};


class StickControl : public IControl
{
public:
	StickControl(EventInput const& eventInput, Action* pAction);
	~StickControl() = default;

	bool IsFlicked() const;
	glm::vec2 GetPos() const;

private:
	glm::vec2 m_pos;
};


#endif
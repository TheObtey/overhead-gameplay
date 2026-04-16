#ifndef FOUNDRY_ICONTROL_H__
#define FOUNDRY_ICONTROL_H__


#include "Define.h"
#include "EventManager.h"


#include <glm/glm.hpp>


class Action;

enum class ControlType : byte
{
	BUTTON = 1,
	SLIDER = 2,
	STICK = 3,

	UNDEFINED = 0
};

enum class ButtonState
{
	PRESSED,
	HOLD,
	RELEASED,
	NONE
};


class IControl /*: public ISerializable*/
{
public:
	IControl() = default;
	IControl(ControlType const& type, Ore::EventInput const& eventInput, Action* pAction = nullptr);
	virtual ~IControl();

	ControlType GetControlType() const;
	Ore::EventInput GetEventInput() const;
	
	template <typename T>
	static T Read(IControl& iControl);

	void SetAction(Action* pAction);

	bool IsPressed();
	bool IsReleased();
	bool IsHold();

private:
	ControlType m_type;
	Ore::EventInput m_eventInput;
	Action* m_pAction; 

	friend Action;
};

class ButtonControl : public IControl
{
public:
	ButtonControl() = default;
	ButtonControl(Ore::EventInput const& eventInput, Action* pAction);
	~ButtonControl() override = default;

	ButtonState GetState() const;
	void SetState(ButtonState state);

private:
	ButtonState m_state;
};


class SliderControl : public IControl
{
public:
	SliderControl() = default;
	SliderControl(Ore::EventInput const& eventInput, Action* pAction);
	~SliderControl() override = default;

	float GetPos() const;
	void SetPos(float pos);

private:
	float m_pos;
};


class StickControl : public IControl
{
public:
	StickControl() = default;
	StickControl(Ore::EventInput const& eventInput, Action* pAction);
	~StickControl() override = default;

	glm::vec2 GetPos() const;
	void SetPos(glm::vec2 pos);

private:
	glm::vec2 m_pos;
};

#endif
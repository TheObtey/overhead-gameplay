#ifndef FOUNDRY_ICONTROL__H_
#define FOUNDRY_ICONTROL__H_

#include "Define.h"

#include <glm/glm.hpp>

enum class EventInput; // forward depuis Ore ?
class Action;


enum class ControlType : byte
{
	BUTTON = 1,
	SLIDER = 2,
	STICK = 3,

	UNDEFINED = 0
};

class IControl
{
public:
	IControl(ControlType const& type, EventInput const& eventInput);
	virtual ~IControl();

	ControlType GetControlType() const;
	
	template <typename T>
	static T Read(IControl& iControl);

	template <typename T>
	T Read();

protected:
	ControlType m_type;
	EventInput m_eventInput;
	Action* m_action; // ptr car sinon pas d'init de la l value
};

class ButtonControl : public IControl
{
public:
	enum class ButtonState : bool;

	ButtonControl(EventInput const& eventInput);
	~ButtonControl() = default;

	ButtonState GetState();

private:
	enum class ButtonState : bool
	{
		UP = false,
		DOWN = true
	} m_state; // UP
};


class SliderControl : public IControl
{
public:
	SliderControl(EventInput const& eventInput);
	~SliderControl() = default;

	float GetPos() const;

private:
	float m_pos; // [-1;1]
};


class StickControl : public IControl
{
public:
	StickControl(EventInput const& eventInput);
	~StickControl() = default;

	bool IsFlicked() const;
	glm::vec2 GetPos() const;

private:
	glm::vec2 m_pos; // [(-1; -1), (1; 1)]
};



#endif
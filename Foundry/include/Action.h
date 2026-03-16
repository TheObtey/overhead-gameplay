#ifndef FOUNDRY_ACTION__H_
#define FOUNDRY_ACTION__H_

#include "Define.h"
#include "Serialization/ISerializable.h"
#include "Event.hpp"

#include <glm/glm.hpp>
#include <vector>

enum class EventInput;

enum class ControlType : byte
{
	BUTTON = 1,
	SLIDER = 2,
	STICK = 3,

	UNDEFINED = 0
};

class Action /*: public ISerializable*/
{
public:
	Action(std::string name = "DEFAULT_ACTION_NAME");
	virtual ~Action();

	template <typename RV, typename... Args>
	Action(
		std::string name, 
		ControlType type, 
		Event<RV, Args...> event,
		EventInput eventInput = NULL
	);
	
	template <typename RV, typename... Args>
	void SetEvent(Event<RV(Args...)> const& event);

	template <typename RV, typename... Args>
	Event<RV(Args...)>& GetEvent(uint32 index) const;

	uint32 AddControl(ControlType const& type, EventInput const& eventInput);

	bool RemoveControl(uint32 const& index);

private:
	std::string m_name;
	std::vector<IControl*> m_controls; 

	Event<void(IControl&)> m_event;
};


class IControl
{
public:
	IControl(ControlType const& type, EventInput const& eventInput);
	virtual ~IControl();

	ControlType GetControlType() const;

	virtual void Read(IControl& iControl) = 0;

protected:
	ControlType m_type;
	EventInput m_eventInput;
	Action* m_action; // ptr car sinon pas d'init de la l value

	friend Action;
};

class ButtonControl : public IControl
{
public:
	enum class ButtonState : bool;

	ButtonControl(EventInput const& eventInput);
	~ButtonControl() override = default;
	
	ButtonState GetState();

	virtual void Read(IControl& iControl) override;

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
	~SliderControl() override = default;

	float GetPos() const;

	virtual void Read(IControl& iControl) override;

private:
	float m_pos; // [-1;1]
};


class StickControl : public IControl
{
public:
	StickControl(EventInput const& eventInput);
	~StickControl() override = default;

	bool IsFlicked() const;
	glm::vec2 GetPos() const;

	virtual void Read(IControl& iControl) override;

private:
	glm::vec2 m_pos; // [(-1; -1), (1; 1)]
};


#endif
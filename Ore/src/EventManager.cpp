#include "EventManager.h"
#include "Logger.hpp"

Event<void(GamepadId)> EventManager::gamepadConnected = Event<void(GamepadId)>();
Event<void(GamepadId)> EventManager::gamepadDisconnected = Event<void(GamepadId)>();

bool EventManager::GetKey(Window window, EventInput key, EventAction event)
{
	int const scancode = glfwGetKeyScancode((int)key);
	const char* ch = glfwGetKeyName((int)key, scancode);
	
	int val = 0;
	if (ch != nullptr)
	{
		val = (int)toupper(ch[0]);
		//Logger::Log(ch[0]);
	}
	else
	{
		val = (int)key;
	}
	int state = glfwGetKey(window.GetWindow(), val);
	if (state == (int)event)
	{
		Logger::LogWithLevel(LogLevel::DEBUG, "TEST");
		return true;
	}
	return false;
}

bool EventManager::GetMouseKey(Window window, EventInput key, EventAction event)
{
	int state = glfwGetMouseButton(window.GetWindow(), (int)key);
	if (state == (int)event)
	{
		Logger::LogWithLevel(LogLevel::DEBUG, (int)key);
		return true;
	}
	return false;
}

bool EventManager::CheckGamepad(GamepadId id)
{
	int present = glfwJoystickPresent((int)id);
	Logger::LogWithLevel(LogLevel::DEBUG, present);
	return present;
}

void EventManager::JoystickCallback(int jId, int event)
{
	if (event == GLFW_CONNECTED)
		EventManager::gamepadConnected((GamepadId)jId);
	if (event == GLFW_DISCONNECTED)
		EventManager::gamepadDisconnected((GamepadId)jId);
}

std::string_view EventManager::GetGamepadName(GamepadId id)
{
	const char* pName = glfwGetGamepadName((int)id);
	std::string_view gamepadName(pName);
	Logger::LogWithLevel(LogLevel::DEBUG, gamepadName);
	return gamepadName;
}

float EventManager::GetGamepadAxes(GamepadId id, EventInput key)
{
	int axesCount;
	const float* pAxes = glfwGetJoystickAxes((int)id, &axesCount);
	Logger::LogWithLevel(LogLevel::DEBUG, pAxes[(int)key]);
	return pAxes[(int)key];
}

bool EventManager::GetButton(GamepadId id, EventInput button)
{
	GLFWgamepadstate state;

	if (glfwGetGamepadState((int)id, &state))
	{
		if (state.buttons[(int)button])
		{
			Logger::LogWithLevel(LogLevel::DEBUG, (int)button);
			return true;
		}
	}
}


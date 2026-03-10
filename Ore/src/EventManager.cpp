#include "EventManager.h"
#include "Logger.hpp"

Event<void(GamepadId)> EventManager::gamepadConnected = Event<void(GamepadId)>();
Event<void(GamepadId)> EventManager::gamepadDisconnected = Event<void(GamepadId)>();

bool EventManager::GetKey(Window window, EventInput key, EventAction event)
{
	int const scancode = glfwGetKeyScancode((int)key);

    if(scancode == GLFW_NOT_INITIALIZED)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "GLFW is not initialized, did you create a window before calling the function ?");
        return false;
    }
    if(scancode == GLFW_INVALID_ENUM)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "The value does not correspond to the enum");
        return false;
    }

	const char* ch = glfwGetKeyName((int)key, scancode);
	
	int val = 0;
	if (ch != nullptr)
	{
		val = (int)toupper(ch[0]);
	}
	else
	{
		val = (int)key;
	}
	int state = glfwGetKey(window.Get(), val);
	if (state == (int)event)
	{
		Logger::LogWithLevel(LogLevel::DEBUG, "TEST");
		return true;
	}
	return false;
}

bool EventManager::GetMouseKey(Window window, EventInput key, EventAction event)
{
	int state = glfwGetMouseButton(window.Get(), (int)key);

	if(state == GLFW_NOT_INITIALIZED)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "GLFW is not initialized, did you create a window before calling the function ?");
        return false;
    }

    if(state == GLFW_INVALID_ENUM)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "The value does not correspond to the enum");
        return false;
    }

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

    if(present == GLFW_NOT_INITIALIZED)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "GLFW is not initialized, did you create a window before calling the function ?");
        return false;
    }

    if(present == GLFW_INVALID_ENUM)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "The value does not correspond to the enum");
        return false;
    }

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

    if(pName == NULL)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Couldn't get gamepad's name at id :", (int)id, "use CheckGamepad first to check if it is present");
        return "";
    }

	std::string_view gamepadName(pName);
	Logger::LogWithLevel(LogLevel::DEBUG, gamepadName);
	return gamepadName;
}

float EventManager::GetGamepadAxes(GamepadId id, EventInput key)
{
	int axesCount;
	const float* pAxes = glfwGetJoystickAxes((int)id, &axesCount);

    if(pAxes == NULL)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Couldn't get joytick's axes at id :", (int)id, "use CheckGamepad first to check if it is present");
        return {};
    }

	Logger::LogWithLevel(LogLevel::DEBUG, pAxes[(int)key]);
	return pAxes[(int)key];
}

bool EventManager::GetButton(GamepadId id, EventInput button)
{
	GLFWgamepadstate state;
    int error = glfwGetGamepadState((int)id, &state);

    if(error == GLFW_FALSE)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Couldn't get gamepad's button at id :", (int)id, "use CheckGamepad first to check if it is present");
        return false;
    }

	if (state.buttons[(int)button])
	{
        Logger::LogWithLevel(LogLevel::DEBUG, (int)button);
		return true;
	}
    return false;
}


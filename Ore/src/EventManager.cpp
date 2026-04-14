#include "EventManager.h"

#include <Logger.hpp>
#include <cctype>


using namespace Ore;
bool EventManager::CheckGamepad(GamepadId id)
{
	uint32 present = glfwJoystickPresent((uint32)id);

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
	const char* pName = glfwGetGamepadName((uint32)id);

    if(pName == NULL)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Couldn't get gamepad's name at id :", (uint32)id, "use CheckGamepad first to check if it is present");
        return "";
    }

	std::string_view gamepadName(pName);
	Logger::LogWithLevel(LogLevel::DEBUG, gamepadName);
	return gamepadName;
}

float EventManager::GetGamepadAxes(GamepadId id, EventInput key)
{
	int axesCount;
	const float* pAxes = glfwGetJoystickAxes((uint32)id, &axesCount);

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

void EventManager::GetKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    int tmp = glfwGetKeyScancode(key);
	const char* ch = glfwGetKeyName(key, tmp);
	
	uint32 val = 0;
    ch != nullptr ? val = static_cast<uint32>(std::toupper(ch[0])) : val = key;

    EventManager::getKey(static_cast<EventInput>(val), static_cast<EventAction>(action));
}

void EventManager::GetMouseButtonCallBack(GLFWwindow *window, int button, int action, int mods)
{
    EventManager::getMouse(static_cast<EventInput>(button), static_cast<EventAction>(action));
}

void EventManager::GetCursorPosCallBack(GLFWwindow *window, double xpos, double ypos)
{
    int32 width = 0;
    int32 height = 0;

    glfwGetWindowSize(window, &width, &height);

    float normalized_x = xpos / width;
    float normalized_y = ypos / height;

    EventManager::getCursorPos(static_cast<float>(normalized_x * 2 - 1.0f), static_cast<float>(-(normalized_y * 2 - 1.0f)));
}
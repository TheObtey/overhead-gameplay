#include "EventManager.h"
#include <iostream>

void hello(GamepadId id)
{
	std::cout << "Hello World" << (int)id << std::endl;
}

void goodbye(GamepadId id)
{
	std::cout << "Goodbye World" << (int)id << std::endl;
}

int main()
{
	Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
	//EventManager::GetGamepadName(GamepadId::GAMEPAD_1);
	//EventManager::GetGamepadName(GamepadId::GAMEPAD_2);
	EventManager::gamepadConnected.Subscribe(hello);
	EventManager::gamepadDisconnected.Subscribe(goodbye);
	while (window.IsOpen())
	{
		EventManager::GetKey(window, EventInput::KEY_A, EventAction::PRESS);
		//EventManager::GetKey(window , EventInput::KEY_Q, EventAction::PRESS);
		//EventManager::GetKey(window , EventInput::KEY_W, EventAction::PRESS);
		//EventManager::GetKey(window , EventInput::KEY_M, EventAction::PRESS);
		//EventManager::GetMouseKey(window , EventInput::MOUSE_RIGHT, EventAction::PRESS);
		//EventManager::CheckGamepad();
		//EventManager::GetGamepadAxes(GamepadId::GAMEPAD_1, EventInput::GAMEPAD_LEFT_X);
		//EventManager::GetButton(GamepadId::GAMEPAD_1, EventInput::GAMEPAD_A);
		//EventManager::GetButton(GamepadId::GAMEPAD_1, EventInput::GAMEPAD_B);
		//EventManager::GetButton(GamepadId::GAMEPAD_1, EventInput::GAMEPAD_X);
		//EventManager::GetButton(GamepadId::GAMEPAD_1, EventInput::GAMEPAD_Y);
		//EventManager::GetButton(GamepadId::GAMEPAD_1, EventInput::GAMEPAD_DPAD_DOWN);
		//EventManager::GetButton(GamepadId::GAMEPAD_1, EventInput::GAMEPAD_LEFT_BUMPER);
		window.Present();
	}
	window.Close();
}

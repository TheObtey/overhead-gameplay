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

void key(EventInput key, EventAction action)
{

    if(key == EventInput::KEY_A && action == EventAction::PRESS)
        std::cout << "oui ça marche oui" << std::endl;
}

void mouse(EventInput mousebutton, EventAction action)
{

    if(mousebutton == EventInput::MOUSE_LEFT && action == EventAction::PRESS)
        std::cout << "en effet cela fonctionne en effet" << std::endl;
}

int main()
{
	Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
	//EventManager::GetGamepadName(GamepadId::GAMEPAD_1);
	//EventManager::GetGamepadName(GamepadId::GAMEPAD_2);
	//EventManager::gamepadConnected.Subscribe(hello);
	//EventManager::gamepadDisconnected.Subscribe(goodbye);
    EventManager::getKey.Subscribe(key);
    EventManager::getMouse.Subscribe(mouse);
	while (window.IsOpen())
	{
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

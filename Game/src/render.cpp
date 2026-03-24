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

void size(uint32 x, uint32 y)
{
    std::cout << "x : " << x << std::endl << "y : "  << y << std::endl;
}

int main()
{
	Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
    window.Open();

    Viewport viewport(0,0,400,600, Color::BLUE);
    Viewport viewport1(400, 0, 400, 600, Color::SKY_BLUE);


    window.AddViewport(viewport);
    window.AddViewport(viewport1);
    window.onResizeEvent.Subscribe(size);

    EventManager::getKey.Subscribe(key);
    EventManager::getMouse.Subscribe(mouse);
    //EventManager::getCursorPos.Subscribe(pos);

	while (window.IsOpen())
	{
        window.Clear();
		window.Present();
	}

	window.Close();
}

#include "Window.h"
#include "Viewport.h"

int main()
{
    Window window2(800, 600, "test");
	Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
    Viewport viewport(0, 0, 400, 600, Color(0.0f,0.0f,1.0f,0.0f), window);
    Viewport view2(400, 0, 400, 600, Color(1.0f,0.0f,0.0f,1.0f), window2);
    
	while (window.IsOpen() && window2.IsOpen())
	{
        viewport.Clear();
        view2.Clear();
        viewport.Present();
        view2.Present();
		window.Present();
        window2.Present();
	}

	window.Close();
}

#include "ActionMap.h"
#include "Action.h"
#include "EventManager.h"

#include <functional>
#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;
Vector2 ballPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };




int main(int argc, char** argv)
{
    ActionMap actionMap = ActionMap();
    
    // Declaration des event ////////////////////////////////////////////////////////////////////////////////////
    Event<void(void)> Zevent; Zevent.Subscribe(std::function<void(void)>([]() { ballPosition.y += 2.0f; }));   //
    actionMap.Emplace("MOVE_UP", new Action("MOVE_UP", ControlType::BUTTON, Zevent, EventInput::KEY_Z));       //
                                                                                                               //
    Event<void(void)> Qevent; Qevent.Subscribe(std::function<void(void)>([]() { ballPosition.x -= 2.0f; }));   //
    actionMap.Emplace("MOVE_LEFT", new Action("MOVE_LEFT", ControlType::BUTTON, Qevent, EventInput::KEY_Q));   //
                                                                                                               //
    Event<void(void)> Sevent; Sevent.Subscribe(std::function<void(void)>([]() { ballPosition.y -= 2.0f; }));   //
    actionMap.Emplace("MOVE_DOWN", new Action("MOVE_DOWN", ControlType::BUTTON, Sevent, EventInput::KEY_S));   //
                                                                                                               //
    Event<void(void)> Devent; Devent.Subscribe(std::function<void(void)>([]() { ballPosition.x += 2.0f; }));   //
    actionMap.Emplace("MOVE_RIGHT", new Action("MOVE_RIGHT", ControlType::BUTTON, Devent, EventInput::KEY_D)); //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //

    ////////////////////////////////////////////////////////////////////////////

    InitWindow(screenWidth, screenHeight, "raylib [core] example - input keys");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Récupération de l'input { Invoke de l'event référencé dans l'actionMap }
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;
        
        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

        DrawCircleV(ballPosition, 50, MAROON);

        EndDrawing();
    }

    CloseWindow();
	return 0;
}

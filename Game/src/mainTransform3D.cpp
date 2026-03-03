////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Define.h"
#include "Node.h"
#include "Servers/EngineServer.h"
#include "Scripting/Lua/LuaScriptInstance.hpp"

#include <memory>
#include "raylib.h"
#include "Transform3D.h"

int main()
{
	std::cout << "main transform 3D" << std::endl;
	// Initialization
   //--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = Vector3{ 10.0f, 10.0f, 10.0f }; // Camera position
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	Mesh cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f);
	Material cubeMaterial = LoadMaterialDefault();

	Vector3 cubePosition = { -2.0f, 0.0f, 0.0f };
	Transform3D cubeTransform;
	Transform3D cube2Transform;
	cube2Transform.SetParent(&cubeTransform);
	cube2Transform.SetPosition({ 2.0,0.0,2.0 });

	DisableCursor();                    // Limit cursor to relative movement inside the window

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())        // Detect window close button or ESC key
	{
		// Events
		if (IsKeyPressed(KEY_Z)) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };

		// move cube
		if (IsKeyDown(KEY_I)) cubeTransform.AddPosition({ 0.0, 0.0, 1.0 });
		if (IsKeyDown(KEY_K)) cubeTransform.AddPosition({ 0.0, 0.0, -1.0 });
		if (IsKeyDown(KEY_J)) cubeTransform.AddPosition({ -1.0, 0.0, 0.0 });
		if (IsKeyDown(KEY_L)) cubeTransform.AddPosition({ 1.0, 0.0, 0.0 });

		if (IsKeyDown(KEY_N)) cubeTransform.AddYaw(0.2);
		if (IsKeyDown(KEY_B)) cubeTransform.AddPitch(0.2);
		if (IsKeyDown(KEY_V)) cubeTransform.AddRoll(0.2);

		if (IsKeyDown(KEY_C)) cubeTransform.AddScale({ 0.0, 0.0, 1.0});
		if (IsKeyDown(KEY_X)) cubeTransform.AddScale({ 0.0, 0.0,-1.0});


		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera, CAMERA_FREE);
		cube2Transform.Update();
		cubeTransform.Update();

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		glm::mat4 m1 = cubeTransform.GetMatrix();
		glm::mat4 m2 = cube2Transform.GetMatrix();

		Matrix rlMat1 = {
			m1[0][0], m1[1][0], m1[2][0], m1[3][0],
			m1[0][1], m1[1][1], m1[2][1], m1[3][1],
			m1[0][2], m1[1][2], m1[2][2], m1[3][2],
			m1[0][3], m1[1][3], m1[2][3], m1[3][3]
		};

		Matrix rlMat2 = {
			m2[0][0], m2[1][0], m2[2][0], m2[3][0],
			m2[0][1], m2[1][1], m2[2][1], m2[3][1],
			m2[0][2], m2[1][2], m2[2][2], m2[3][2],
			m2[0][3], m2[1][3], m2[2][3], m2[3][3]
		};

		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = RED;
		DrawMesh(cubeMesh, cubeMaterial, rlMat1);

		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
		DrawMesh(cubeMesh, cubeMaterial, rlMat2);

		DrawGrid(10, 1.0f);

		EndMode3D();

		DrawRectangle(10, 10, 320, 93, Fade(SKYBLUE, 0.5f));
		DrawRectangleLines(10, 10, 320, 93, BLUE);

		DrawText("Free camera default controls:", 20, 20, 10, BLACK);
		DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
		DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
		DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------


}

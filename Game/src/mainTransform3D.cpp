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

struct Node3D
{
	Transform3D transform;
};

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

	Node3D cube1;
	Node3D cube2;
	Node3D cube3;
	Node3D cube4;
	auto& tr1 = cube1.transform;
	auto& tr2 = cube2.transform;
	auto& tr3 = cube3.transform;
	auto& tr4 = cube4.transform;

	tr2.SetPosition({ 4.0,0.0,2.0 });
	tr3.SetPosition({ -4.0,0.0,-2.0 });
	tr4.SetPosition({ -2.0,2.0,-2.0 });
	tr2.SetScale({ 3.0,0.5,2.0 });

	DisableCursor();                    // Limit cursor to relative movement inside the window

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())        // Detect window close button or ESC key
	{
		// Events
		if (IsKeyPressed(KEY_Z)) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };

		// move cube
		if (IsKeyDown(KEY_I)) tr1.AddPosition({ 0.0, 0.0, 1.0 });
		if (IsKeyDown(KEY_K)) tr1.AddPosition({ 0.0, 0.0, -1.0 });
		if (IsKeyDown(KEY_J)) tr1.AddPosition({ -1.0, 0.0, 0.0 });
		if (IsKeyDown(KEY_L)) tr1.AddPosition({ 1.0, 0.0, 0.0 });
		if (IsKeyDown(KEY_G)) tr2.AddPosition({ 1.0, 0.0, 0.0 });

		if (IsKeyDown(KEY_N)) tr1.AddYaw(0.2);
		if (IsKeyDown(KEY_B)) tr1.AddPitch(0.2);
		if (IsKeyDown(KEY_V)) tr1.AddRoll(0.2);

		if (IsKeyDown(KEY_C)) tr1.AddScale({ 1.0, 1.0, 1.0});
		if (IsKeyDown(KEY_X)) tr1.AddScale({ -1.0, -1.0, -1.0});

		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera, CAMERA_FREE);
		tr1.Update();
		tr2.Update();
		tr3.Update();
		tr4.Update();

		tr2 = tr2 * tr1;
		tr3 = tr3 * tr1;
		tr4 = tr4 * tr3;

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		glm::mat4& m1 = tr1.GetMatrix();
		glm::mat4& m2 = tr2.GetMatrix();
		glm::mat4& m3 = tr3.GetMatrix();
		glm::mat4& m4 = tr4.GetMatrix();

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

		Matrix rlMat3 = {
			m3[0][0], m3[1][0], m3[2][0], m3[3][0],
			m3[0][1], m3[1][1], m3[2][1], m3[3][1],
			m3[0][2], m3[1][2], m3[2][2], m3[3][2],
			m3[0][3], m3[1][3], m3[2][3], m3[3][3]
		};

		Matrix rlMat4 = {
			m4[0][0], m4[1][0], m4[2][0], m4[3][0],
			m4[0][1], m4[1][1], m4[2][1], m4[3][1],
			m4[0][2], m4[1][2], m4[2][2], m4[3][2],
			m4[0][3], m4[1][3], m4[2][3], m4[3][3]
		};

		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = RED;
		DrawMesh(cubeMesh, cubeMaterial, rlMat1);

		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
		DrawMesh(cubeMesh, cubeMaterial, rlMat2);

		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
		DrawMesh(cubeMesh, cubeMaterial, rlMat3);

		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
		DrawMesh(cubeMesh, cubeMaterial, rlMat4);

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

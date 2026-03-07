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
#include "Nodes/Node3D.h"


int main()
{
	std::cout << "main transform 3D" << std::endl;
	// Initialization
   //--------------------------------------------------------------------------------------
	const int screenWidth = 1500;
	const int screenHeight = 850;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = Vector3{ 10.0f, 10.0f, 10.0f }; // Camera position
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
	Material cubeMaterial = LoadMaterialDefault();

	auto cube1 = Node::CreateNode<Node3D>("a");
	auto cube2 = Node::CreateNode<Node3D>("b");
	auto cube3 = Node::CreateNode<Node3D>("c");
	auto cube4 = Node::CreateNode<Node3D>("y");

	cube2->SetPosition({ 1.0,0.0,0.0});
	cube3->SetPosition({ 2.0,0.0,0.0});
	cube4->SetPosition({ 1.0,0.0,0.0});

	cube2->SetScale({ 1.0,4.0,1.0});

	cube1->AddChild(std::move(cube2));
	cube1->AddChild(std::move(cube3));

	EngineServer::FlushCommands();
	Node3D& cube1ref = *cube1.get();
	Node3D& cube2ref =  static_cast<Node3D&>(cube1->GetChild(0));
	Node3D& cube3ref =  static_cast<Node3D&>(cube1->GetChild(1));

	cube3ref.AddChild(std::move(cube4));
	EngineServer::FlushCommands();
	Node3D& cube4ref =  static_cast<Node3D&>(cube3ref.GetChild(0));

	DisableCursor();                    // Limit cursor to relative movement inside the window

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	EnableCursor();
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())        // Detect window close button or ESC key
	{
		// Events
		if (IsKeyPressed(KEY_Z)) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };

		// move cube
		if (IsKeyDown(KEY_I))
			cube2ref.SetWorldPosition(cube2ref.GetWorldPosition() + glm::vec3(0.0f, 0.0f, -0.5f));
		if (IsKeyDown(KEY_K)) cube1ref.AddPosition({ 0.0, 0.0, -1.0 });
		if (IsKeyDown(KEY_J)) cube1ref.AddPosition({ -1.0, 0.0, 0.0 });
		if (IsKeyDown(KEY_L)) cube1ref.AddPosition({ 1.0, 0.0, 0.0 });
		if (IsKeyDown(KEY_G)) cube2ref.AddPosition({ 1.0, 0.0, 0.0 });

		if (IsKeyDown(KEY_RIGHT)) cube2ref.AddPosition({ 1.0, 0.0, 0.0 });
		if (IsKeyDown(KEY_UP)) cube2ref.AddRoll(0.2);

		if (IsKeyDown(KEY_N)) cube1ref.AddYaw(0.2);
		if (IsKeyDown(KEY_B)) cube1ref.AddPitch(0.2);
		if (IsKeyDown(KEY_V)) cube1ref.AddRoll(0.2);

		if (IsKeyPressed(KEY_C)) cube1ref.AddScale({ 0.0, 1.0, 0.0});
		if (IsKeyDown(KEY_X)) cube1ref.AddScale({ -1.0, -1.0, -1.0});

		if (IsKeyPressed(KEY_Y))
		{
			cube4ref.Reparent(cube1ref.GetChild(0));
		}

		// Update
		//----------------------------------------------------------------------------------
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			UpdateCamera(&camera, CAMERA_FREE);

		cube1->Update(0.016f);


		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		glm::mat4 m1 = cube1->GetWorldMatrix();
		glm::mat4 m2 = cube2ref.GetWorldMatrix();
		glm::mat4 m3 = cube3ref.GetWorldMatrix();
		glm::mat4 m4 = cube4ref.GetWorldMatrix();

		//DEBUG(m4[0][])

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

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
}

#include <glm/glm.hpp>
#include <memory>
#include <iostream>
#include <raylib.h>

#include "Define.h"
#include "Node.h"
#include "Servers/EngineServer.h"
#include "Scripting/Lua/LuaScriptInstance.hpp"

#include "Nodes/Node2D.h"

int main(int argc, char** argv)
{
	Camera3D camera = { 0 };
	camera.position = { 0.0f, 0.0f, -10.0f };  // Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = { 0.0f, -1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_ORTHOGRAPHIC;            // Camera 

	InitWindow(800, 600, "Test Node2D");
	
	Mesh squareMesh = GenMeshCube(1.0f, 1.0f, 1.0f);

	Material cubeMaterial1 = LoadMaterialDefault();
	cubeMaterial1.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
	Material cubeMaterial2 = LoadMaterialDefault();
	cubeMaterial2.maps[MATERIAL_MAP_DIFFUSE].color = RED;

	auto parent = Node::CreateNode<Node2D>("Parent");
	parent->SetPosition({ 0.0f, 0.0f });
	auto child = Node::CreateNode<Node2D>("Child");
	child->SetPosition({ 5.0f, 0.0f });

	parent->AddChild(std::move(child)); 

	EngineServer::FlushCommands();

	Node2D& childRef = static_cast<Node2D&>(parent->GetChild(0));

	EnableCursor();
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	while (WindowShouldClose() == false)
	{
		if (IsKeyDown(KEY_D))
		{
			parent->SetPosition(parent->GetPosition().x + 0.1f, parent->GetPosition().y);
		}
		if (IsKeyDown(KEY_Q))
		{
			parent->SetPosition(parent->GetPosition().x - 0.1f, parent->GetPosition().y);
		}
		if (IsKeyDown(KEY_Z))
		{
			parent->SetWorldPosition(parent->GetWorldPosition() + glm::vec3(0.0, 0.1f, 0.0));
		}
		if (IsKeyDown(KEY_S))
		{
			childRef.SetWorldPosition(childRef.GetWorldPosition() - glm::vec3(0.0, 0.1f, 0.0));

		}
		if (IsKeyDown(KEY_A))
		{
			parent->SetRotation(parent->GetRotation() + 0.1f);
		}
		if (IsKeyDown(KEY_E))
		{
			parent->SetRotation(parent->GetRotation() - 0.1f);
		}

		if (IsKeyDown(KEY_O))
		{
			parent->SetScale(parent->GetScale().x + 1.0f, parent->GetScale().y + 1.0f);
		}
		if (IsKeyDown(KEY_P))
		{
			parent->SetScale(parent->GetScale().x - 1.0f, parent->GetScale().y - 1.0f);
		}

		if (IsKeyDown(KEY_V))
		{
			parent->SetMirroringOnAxis(Transform2D::Axis::Y);
		}

		if (IsKeyDown(KEY_B))
		{
			parent->SetShearing(parent->GetShearing() + glm::vec2(0.01f, 0.0f));
		}

		parent->Update(0.0017);
		//parent->UpdateLocal();
		//parent->UpdateWorld();
		//childRef.UpdateWorld();

		glm::mat3 const& m1 = parent->GetTransformationMatrix();
		glm::mat3 const& m2 = childRef.GetTransformationMatrix();

		Matrix rlMat1 = {
			m1[0][0], m1[1][0],   0.0f,    m1[2][0],
			m1[0][1], m1[1][1],   0.0f,    m1[2][1],
			  0.0f,     0.0f,     1.0f,    0.0f,
			  0.0f,     0.0f,     0.0f,    1.0f
		};
		
		Matrix rlMat2 = {
			m2[0][0], m2[1][0],   0.0f,    m2[2][0],
			m2[0][1], m2[1][1],   0.0f,    m2[2][1],
			  0.0f,     0.0f,     1.0f,    0.0f,
			  0.0f,     0.0f,     0.0f,    1.0f
		};


		BeginDrawing();
			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
				DrawMesh(squareMesh, cubeMaterial1, rlMat1); // BLUE
				DrawMesh(squareMesh, cubeMaterial2, rlMat2); // RED
			//DrawCube({ 0, 0, 0 }, 2.0f, 2.0f, 2.0f, RED);
			EndMode3D();


			
			DrawText("debug", 100, 100, 20, RED);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}



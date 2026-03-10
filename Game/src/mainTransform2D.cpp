#include <glm/glm.hpp>
#include <memory>
#include <iostream>
#include <raylib.h>

#include "Define.h"
#include "Node.h"
#include "Servers/EngineServer.h"
#include "Scripting/Lua/LuaScriptInstance.hpp"

#include "Nodes/Node2D.h"

Vector2 TransformPoint(glm::mat3 const& _mat, Vector2 _pos)
{
	return {
		_mat[0][0] * _pos.x + _mat[0][1] * _pos.x + _mat[0][2] * _pos.x,
		_mat[1][0] * _pos.x + _mat[1][1] * _pos.x + _mat[1][2] * _pos.x
	};
}



int main(int argc, char** argv)
{
	Camera3D camera = { 0 };
	camera.position = { 10.0f, 10.0f, 10.0f };			// Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };				// Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };					// Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	InitWindow(800, 600, "Test Node2D");
	
	Mesh squareMesh = GenMeshPlane(30.0f, 40.0f, 20, 20);
	Material cubeMaterial = LoadMaterialDefault();
	cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = PURPLE;

	Node2D node2D(100.0f, 100.0f, 1.0f, 1.0f, false);
	
	DisableCursor();
	SetTargetFPS(60);

	while (WindowShouldClose() == false)
	{
		if (IsKeyDown(KEY_D))
		{
			node2D.SetPosition(node2D.GetPosition().x + 10.0f, node2D.GetPosition().y);
		}
		if (IsKeyDown(KEY_Q))
		{
			node2D.SetPosition(node2D.GetPosition().x - 10.0f, node2D.GetPosition().y);
		}
		if (IsKeyDown(KEY_Z))
		{
			node2D.SetPosition(node2D.GetPosition().x, node2D.GetPosition().y + 10.0f);
		}
		if (IsKeyDown(KEY_S))
		{
			node2D.SetPosition(node2D.GetPosition().x, node2D.GetPosition().y - 10.0f);
		}
		
		if (IsKeyDown(KEY_A))
		{
			node2D.SetRotation(node2D.GetRotation().x + 1.0f, node2D.GetRotation().y);
		}
		if (IsKeyDown(KEY_E))
		{
			node2D.SetRotation(node2D.GetRotation().x, node2D.GetRotation().y + 1.0f);
		}

		if (IsKeyDown(KEY_O))
		{
			node2D.SetScale(node2D.GetScale().x + 1.0f, node2D.GetScale().y + 1.0f);
		}
		if (IsKeyDown(KEY_P))
		{
			node2D.SetScale(node2D.GetScale().x - 1.0f, node2D.GetScale().y - 1.0f);
		}

		//node2D.Update(0.0017);
		//node2D.UpdateLocal();
		node2D.UpdateWorld();

		glm::mat3& m1 = node2D.GetTransformationMatrix();

		Matrix rlMat1 = {
			m1[0][0], m1[0][1],    0.0f,    0.0f,
			m1[1][0], m1[1][1],    0.0f,    0.0f,
			  0.0f,     0.0f,      0.0f,    0.0f,
			m1[2][0], m1[2][1], m1[2][2],   1.0f
		};



		BeginDrawing();
			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
				DrawMesh(squareMesh, cubeMaterial, rlMat1);
			EndMode3D();


			
			DrawText("debug", 100, 100, 20, RED);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}



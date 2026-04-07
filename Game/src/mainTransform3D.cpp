////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Define.h"
#include "Node.h"
#include "Servers/EngineServer.h"
#include "Servers/PhysicsServer.h"
#include "Scripting/Lua/LuaScriptInstance.hpp"

#include <memory>
#include <reactphysics3d/reactphysics3d.h>
#include "raylib.h"
#include "Transform3D.h"
#include "Nodes/Node3D.h"
#include "Nodes/NodeRigidBody.h"
#include <Nodes/NodeCollider.h>
#include "Clock.hpp"
#include <Debug.h>


//class OverlapCB : public rp3d::OverlapCallback
//{
//public:
//	bool overlapp = false;
//	void onOverlap(OverlapCallback::CallbackData& data) override
//	{
//		uint32 nbPairs = data.getNbOverlappingPairs();
//
//		overlapp = (nbPairs > 0);
//		for (int i = 0; i < nbPairs; i++)
//		{
//			OverlapPair pair = data.getOverlappingPair(i);
//			auto eventType = pair.getEventType();
//			if (eventType == OverlapPair::EventType::OverlapStart)
//				DEBUG("Start trigger\n");
//			else if (eventType == OverlapPair::EventType::OverlapStay)
//				DEBUG("In trigger\n");
//			else if (eventType == OverlapPair::EventType::OverlapExit)
//				DEBUG("Got out of trigger\n");
//		}
//	}
//};
//class CollisionCB : public rp3d::CollisionCallback
//{
//public:
//	bool hit = false;
//	void onContact(const CollisionCallback::CallbackData& data) override
//	{
//		uint32 nbPairs = data.getNbContactPairs();
//		//m_contacts.resize(nbPairs);
//		hit = (nbPairs > 0);
//		for (int i = 0; i < nbPairs; i++)
//		{
//			ContactPair pair = data.getContactPair(i);
//
//			auto eventType = pair.getEventType();
//			if (eventType == ContactPair::EventType::ContactStart)
//				DEBUG("Start Contact\n");
//			else if (eventType == ContactPair::EventType::ContactStay)
//				DEBUG("In Contact\n");
//			else if (eventType == ContactPair::EventType::ContactExit)
//				DEBUG("Got out of Contact\n");
//			//m_contacts.push_back(pair);
//		}
//		uint32 nbContacts = data.getNbContactPairs();
//	}
//};
 
// Class events to redefine for custom collision and trigger events
class PhysicsEventz : public rp3d::EventListener
{
public:
	virtual void onContact(const rp3d::CollisionCallback::CallbackData& data) = 0;
	virtual void onTrigger(const rp3d::OverlapCallback::CallbackData& data) = 0;
};


class Events : public PhysicsEventz
{
public:
	bool hit = false;
	bool overlapp = false;


	// OVERLAP WITH COLLIDERS
	void onContact(const rp3d::CollisionCallback::CallbackData& data) override
	{
		uint32 nbPairs = data.getNbContactPairs();
		hit = (nbPairs > 0);
		for (int i = 0; i < nbPairs; i++)
		{
			//ContactPair pair = data.getContactPair(i);

			//auto eventType = pair.getEventType();
			//if (eventType == ContactPair::EventType::ContactStart)
			//	DEBUG("Start Contact\n");
			//else if (eventType == ContactPair::EventType::ContactStay)
			//	DEBUG("In Contact\n");
			//else if (eventType == ContactPair::EventType::ContactExit)
			//	DEBUG("Got out of Contact\n");
		}
	}

	// OVERLAP WITH TRIGGERS
	void onTrigger(const rp3d::OverlapCallback::CallbackData& data) override
	{
		auto nbPairs = data.getNbOverlappingPairs();
		overlapp = (nbPairs > 0);
		for (int i = 0; i < nbPairs; i++)
		{
			using OverlapPair = rp3d::OverlapCallback::OverlapPair;
			OverlapPair pair = data.getOverlappingPair(i);

			auto eventType = pair.getEventType();
			if (eventType == OverlapPair::EventType::OverlapStart)
				std::cout << "Start trigger\n";
			else if (eventType == OverlapPair::EventType::OverlapStay)
				std::cout << "In trigger\n";
			else if (eventType == OverlapPair::EventType::OverlapExit)
				std::cout << "Got out of trigger\n";
		}
	}
	void Reset()
	{
		hit = false;
		overlapp = false;
	}
};

// example of collision masks
enum CollisionMasks
{
	MASK_NONE			= 0x0000,
	MASK_PLAYER			= 0x0001,
	MASK_ENVIRONMENT	= 0x0002,
	MASK_TRIGGER		= 0x0003,
};



void DrawOBBWires(Vector3 halfExtents, Matrix rotation, Color color)
{
	// vertices
	Vector3 corners[8] = {
		{ -halfExtents.x, -halfExtents.y, -halfExtents.z },
		{  halfExtents.x, -halfExtents.y, -halfExtents.z },
		{  halfExtents.x,  halfExtents.y, -halfExtents.z },
		{ -halfExtents.x,  halfExtents.y, -halfExtents.z },
		{ -halfExtents.x, -halfExtents.y,  halfExtents.z },
		{  halfExtents.x, -halfExtents.y,  halfExtents.z },
		{  halfExtents.x,  halfExtents.y,  halfExtents.z },
		{ -halfExtents.x,  halfExtents.y,  halfExtents.z },
	};

	// rotate vertices
	for (int i = 0; i < 8; i++)
	{

		glm::mat4 rotMat = {
			rotation.m0, rotation.m1, rotation.m2, rotation.m3,
			rotation.m4, rotation.m5, rotation.m6, rotation.m7,
			rotation.m8, rotation.m9, rotation.m10, rotation.m11,
			rotation.m12, rotation.m13, rotation.m14, rotation.m15
		};
		auto rotVec = glm::vec4(corners[i].x, corners[i].y, corners[i].z, 1.0f);
		rotVec = rotMat * rotVec;

		corners[i] = { rotVec.x, rotVec.y, rotVec.z };
	}

	// 12 arêtes du cube (paires d'indices)
	int edges[12][2] = {
		{0,1},{1,2},{2,3},{3,0},  // face arrière
		{4,5},{5,6},{6,7},{7,4},  // face avant
		{0,4},{1,5},{2,6},{3,7}   // arêtes latérales
	};

	for (int i = 0; i < 12; i++) {
		DrawLine3D(corners[edges[i][0]], corners[edges[i][1]], color);
	}
}

int main() {
	InitWindow(1280, 720, "Cube");
	SetTargetFPS(60);

	Camera3D cam = {};
	cam.position = { 0.0f, 20.0f, -40.0f };
	cam.target = { 0.0f,  5.0f,  0.0f };
	cam.up = { 0.0f,  1.0f,  0.0f };
	cam.fovy = 60.0f;
	cam.projection = CAMERA_PERSPECTIVE;

	PhysicsServer::Initialize();

	//// player
	glm::vec3 pos = { 0.0f, 10.0f, 0.0f };
	glm::vec3 pSize = { 1.0f, 1.0f, 1.0f };
	float   rotY = 0.0f;
	float   rotX = 0.0f;

	auto player = Node::CreateNode<Node3D>("player");
	player.get()->SetLocalPosition(pos);
	

	PhysicsServer::FlushCommands();
	bool test = false;
	bool t = false;
	while (!WindowShouldClose())
	{
																				

		if (IsKeyDown(KEY_C))  player.get()->SetLocalPosition({ player.get()->GetWorldPosition().x +1, player.get()->GetWorldPosition().y, player.get()->GetWorldPosition().z });

		/
		player->Update(dt);
		

		// --- draw ---
		BeginDrawing();
		ClearBackground({ 20, 20, 30, 255 });

		BeginMode3D(cam);
		DrawGrid(30, 1.0f);

		glm::mat4 mat_player = player->GetWorldMatrix();

		auto getMat = [](glm::mat4 const& m) -> Matrix
			{
				return {
					m[0][0], m[1][0], m[2][0], m[3][0],
					m[0][1], m[1][1], m[2][1], m[3][1],
					m[0][2], m[1][2], m[2][2], m[3][2],
					m[0][3], m[1][3], m[2][3], m[3][3]
				};
			};

		Matrix rlMat_player = getMat(mat_player);


		auto draw = [&](Matrix const& mat, Color c)
			{
				cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = c;
				DrawMesh(cubeMesh, cubeMaterial, mat);
			};

		draw(rlMat_player, isColliding ? RED : BEIGE);
		DrawOBBWires(
			{ 1.0f, 1.0f, 1.0f },
			rlMat_player,
			BLACK
		);


		// Mur
		DrawCube(WALL_POS, WALL_W, WALL_H, WALL_D, { 180, 140, 80, 255 });
		DrawCubeWires(WALL_POS, WALL_W, WALL_H, WALL_D, DARKBROWN);
		// Mur
		DrawCube(TRIGGER_POS, TRIGGER_W, TRIGGER_H, TRIGGER_D, YELLOW);
		DrawCubeWires(TRIGGER_POS, TRIGGER_W, TRIGGER_H, TRIGGER_D, DARKBROWN);

		DrawCube(FLOOR_POS, FLOOR_W, FLOOR_H, FLOOR_D, { 130, 140, 95, 255 });
		DrawCubeWires(FLOOR_POS, FLOOR_W, FLOOR_H, FLOOR_D, DARKBROWN);

		EndMode3D();


		// HUD
		if (isColliding)
			DrawText(">>> COLLISION <<<", 20, 118, 20, RED);
		else
			DrawText("Aucune collision", 20, 118, 20, GREEN);

		DrawText(TextFormat("C_Pos: (%.1f, %.1f, %.1f)", ref_playerCollider->GetLocalPosition().x, ref_playerCollider->GetLocalPosition().y, ref_playerCollider->GetLocalPosition().z),
			10, GetScreenHeight() - 70, 16, BLACK);
		DrawText(TextFormat("RB_Pos: (%.1f, %.1f, %.1f)", ref_playerRB->GetPosition().x, ref_playerRB->GetPosition().y, ref_playerRB->GetPosition().z),
			10, GetScreenHeight() - 48, 16, BLACK);
		DrawText(TextFormat("PlayerPos: (%.1f, %.1f, %.1f)", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z),
			10, GetScreenHeight() - 26, 16, BLACK);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

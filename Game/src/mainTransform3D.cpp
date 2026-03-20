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


class OverlapCB : public rp3d::OverlapCallback {
public:
	bool hit = false;
	void onOverlap(CallbackData& data) override {
		hit = (data.getNbOverlappingPairs() > 0);
	}
};
class CollisionCB : public rp3d::CollisionCallback {
public:
	bool hit = false;
	void onContact(const rp3d::CollisionCallback::CallbackData& data) override {
		hit = (data.getNbContactPairs() > 0);
	}
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
	cam.target = { 0.0f,  0.0f,  0.0f };
	cam.up = { 0.0f,  1.0f,  0.0f };
	cam.fovy = 50.0f;
	cam.projection = CAMERA_PERSPECTIVE;

	PhysicsServer::Initialize();

	//// cube
	glm::vec3 pos = { 0.0f, 1.0f, 0.0f };
	glm::vec3 pSize = { 1.0f, 1.0f, 1.0f };
	float   rotY = 0.0f;
	float   rotX = 0.0f;

	auto player = Node::CreateNode<Node3D>("player");
	auto RB_player = Node::CreateNode<NodeRigidBody>("rb_player");
	auto C_player = Node::CreateNode<NodeCollider>("c_player");

	player->SetLocalPosition(pos);

	C_player->SetBoxShape(pSize);

	RB_player->SetNode3DParent(player.get());
	RB_player->SetBodyType(RigidBodyType::DYNAMIC);
	RB_player->SetIsGravityEnabled(true);
	RB_player->SetMass(1.0f);
	RB_player->SetAngularDamping(2.0f);
	RB_player->SetLinearDamping(1.0f);

	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider
	RB_player->AddChild(std::move(C_player));


	//  Mur statique 
	const Vector3   WALL_POS= { 3.0f, 1.0f, 0.0f };
	const float     WALL_W	= 1.0f, WALL_H = 4.0f, WALL_D = 6.0f;
	auto			wall	= Node::CreateNode<Node3D>("wall");
	auto			RB_wall = Node::CreateNode<NodeRigidBody>("rb_wall");
	auto			C_wall	= Node::CreateNode<NodeCollider>("c_wall");

	wall->SetLocalPosition({ 3.0f, 1.0f, 0.0f });

	RB_wall->SetNode3DParent(wall.get());
	RB_wall->SetBodyType(RigidBodyType::STATIC);

	C_wall->SetBoxShape({ 0.5f, 2.0f, 3.0f });

	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider
	RB_wall->AddChild(std::move(C_wall));


	//  Sol statique 
	const Vector3   FLOOR_POS = { 0.0f, -1.0f, 0.0f };
	const float     FLOOR_W = 100.0f, FLOOR_H = 2.0f, FLOOR_D = 100.0f;
	auto			floor	= Node::CreateNode<Node3D>("floor");
	auto			RB_floor= Node::CreateNode<NodeRigidBody>("rb_floor");
	auto			C_floor = Node::CreateNode<NodeCollider>("c_floor");

	floor->SetLocalPosition({ 0.0f, -1.0f, 0.0f });
	C_floor->SetBoxShape({ 50.0f, 1.0f, 50.0f });

	RB_floor->SetNode3DParent(floor.get());
	RB_floor->SetBodyType(RigidBodyType::STATIC);

	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider
	RB_floor->AddChild(std::move(C_floor));


	// params
	const float SPEED = 4.0f;
	const float ROT_SPEED = 90.0f;
	const float FIXED_DT = 1.0f / 60.0f;
	float accumulator = 0.0f;
	bool  isColliding = false;

	Mesh cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f);
	Material cubeMaterial = LoadMaterialDefault();

	EngineServer::FlushCommands(); // Important pour que les RB soient tous cree avant de faire le cast des colliders

	auto ref_collider = static_cast<NodeCollider*>(&RB_player->GetChild(0));
	auto ref_wallCollider = static_cast<NodeCollider*>(&RB_wall->GetChild(0));
	auto ref_floorCollider = static_cast<NodeCollider*>(&RB_floor->GetChild(0));
	
	player->AddScale({ 1.0f, 3.0f, 0.0f });
	ref_collider->SetBoxShape(player->GetScale());

	bool test = false;
	bool t = false;
	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		if (dt <= 0.0f || dt > 0.1f) dt = FIXED_DT;

		if (IsKeyDown(KEY_W)) RB_player->ApplyWorldForceAtCenterOfMass({ 0, 0, 50 }); // Z
		if (IsKeyDown(KEY_S)) RB_player->ApplyWorldForceAtCenterOfMass({ 0, 0, -50 }); // S
		if (IsKeyDown(KEY_D)) RB_player->ApplyWorldForceAtCenterOfMass({ -50, 0, 0 }); // D
		if (IsKeyDown(KEY_A))
		{
			RB_player->ApplyWorldForceAtCenterOfMass({ 50, 0, 0 }); // Q
		}
		if (IsKeyDown(KEY_Q)) RB_player->ApplyWorldForceAtCenterOfMass({ 0, 100, 0 }); // A
		if (IsKeyDown(KEY_E)) RB_player->ApplyWorldForceAtCenterOfMass({ 0, -100, 0 });	// E



		if (IsKeyDown(KEY_V))  RB_player->ApplyLocalTorque({ 50,0,0 });
		if (IsKeyDown(KEY_B)) RB_player->ApplyLocalTorque({ 0,50,0 });
		if (IsKeyDown(KEY_N))  RB_player->ApplyLocalTorque({ 0,0,50 });

		if (IsKeyDown(KEY_C))  RB_player->ResetTorque();

		if (IsKeyPressed(KEY_G))
		{
			test = !test;
			RB_player->LockAngularAxis(test, test, test);
			std::string a = test == true ? "true" : "false";
			std::string b = "\n Linear lock = " + a + "\n";
			DEBUG(b);
		}
		if (IsKeyPressed(KEY_H))
		{
			t = !t;
			RB_player->LockLinearAxis(t, t, t);
			std::string a = t == true ? "true" : "false";
			std::string b = "\n Angular lock = " + a + "\n";
			DEBUG(b);
		}

		player->Update(dt);
		RB_player->Update(dt);

		// --- update physics ---
		accumulator += dt;
		while (accumulator >= FIXED_DT) {

			PhysicsServer::UpdatePhysicsWorld(FIXED_DT);
			accumulator -= FIXED_DT;
		}

		PhysicsServer::FlushCommands();

		// --- test overlap ---
		OverlapCB cb;
		PhysicsServer::GetPhysicsWorld().testOverlap(RB_player->GetRigidBody(), cb);
		isColliding = cb.hit;

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

		DrawCube(FLOOR_POS, FLOOR_W, FLOOR_H, FLOOR_D, { 130, 140, 95, 255 });
		DrawCubeWires(FLOOR_POS, FLOOR_W, FLOOR_H, FLOOR_D, DARKBROWN);

		EndMode3D();


		// HUD
		if (isColliding)
			DrawText(">>> COLLISION <<<", 20, 118, 20, RED);
		else
			DrawText("Aucune collision", 20, 118, 20, GREEN);

		DrawText(TextFormat("C_Pos: (%.1f, %.1f, %.1f)", ref_collider->GetLocalPosition().x, ref_collider->GetLocalPosition().y, ref_collider->GetLocalPosition().z),
			10, GetScreenHeight() - 70, 16, BLACK);
		DrawText(TextFormat("RB_Pos: (%.1f, %.1f, %.1f)", RB_player->GetPosition().x, RB_player->GetPosition().y, RB_player->GetPosition().z),
			10, GetScreenHeight() - 48, 16, BLACK);
		DrawText(TextFormat("PlayerPos: (%.1f, %.1f, %.1f)", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z),
			10, GetScreenHeight() - 26, 16, BLACK);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

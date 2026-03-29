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
	glm::vec3 pos = { 0.0f, 6.0f, 0.0f };
	glm::vec3 pSize = { 1.0f, 1.0f, 1.0f };
	float   rotY = 0.0f;
	float   rotX = 0.0f;

	auto player = Node::CreateNode<Node3D>("player");
	auto RB_player = Node::CreateNode<NodeRigidBody>("rb_player");
	auto C_player = Node::CreateNode<NodeBoxCollider>("c_player");

	player->SetLocalPosition(pos);
	PhysicsServer::FlushCommands(); 
	player->AddChild(std::move(RB_player));
	EngineServer::FlushCommands(); 

	auto ref_playerRB = static_cast<NodeRigidBody*>(&player->GetChild(0));

	C_player->SetShape(pSize);

	//ref_playerRB->SetNode3DParent(player.get());
	ref_playerRB->SetBodyType(RigidBodyType::DYNAMIC);
	ref_playerRB->SetIsGravityEnabled(true);
	ref_playerRB->SetMass(3.0f);
	ref_playerRB->SetAngularDamping(2.0f);
	ref_playerRB->SetLinearDamping(2.0f);

	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider
	ref_playerRB->AddChild(std::move(C_player));


	//  Mur statique 
	const Vector3   WALL_POS = { 3.0f, 1.0f, 0.0f };
	const float     WALL_W = 1.0f, WALL_H = 4.0f, WALL_D = 6.0f;
	auto			wall = Node::CreateNode<Node3D>("wall");
	auto			RB_wall = Node::CreateNode<NodeRigidBody>("rb_wall");
	auto			C_wall = Node::CreateNode<NodeBoxCollider>("c_wall");

	wall->SetLocalPosition({ 3.0f, 1.0f, 0.0f });
	PhysicsServer::FlushCommands(); 
	wall->AddChild(std::move(RB_wall));
	EngineServer::FlushCommands(); 
	auto ref_wallRB = static_cast<NodeRigidBody*>(&wall->GetChild(0));

	//RB_wall->SetNode3DParent(wall.get());
	ref_wallRB->SetBodyType(RigidBodyType::STATIC);

	C_wall->SetShape({ 0.5f, 2.0f, 3.0f });

	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider
	ref_wallRB->AddChild(std::move(C_wall));

	//  Zone Trigger statique 
	const Vector3   TRIGGER_POS = { -3.0f, -1.0f, 0.0f };
	const float     TRIGGER_W = 1.0f, TRIGGER_H = 4.0f, TRIGGER_D = 6.0f;
	auto			trigger = Node::CreateNode<Node3D>("trigger");
	auto			RB_trigger = Node::CreateNode<NodeRigidBody>("rb_trigger");
	auto			C_trigger = Node::CreateNode<NodeBoxCollider>("c_trigger");

	trigger->SetLocalPosition({ -3.0f, -1.0f, 0.0f });
	PhysicsServer::FlushCommands(); 
	trigger->AddChild(std::move(RB_trigger));
	EngineServer::FlushCommands(); 
	auto ref_triggerRB = static_cast<NodeRigidBody*>(&trigger->GetChild(0));

	//RB_trigger->SetNode3DParent(trigger.get());
	ref_triggerRB->SetBodyType(RigidBodyType::STATIC);

	C_trigger->SetShape({ 0.5f, 2.0f, 3.0f });

	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider
	ref_triggerRB->AddChild(std::move(C_trigger));


	//  Sol statique 
	const Vector3   FLOOR_POS = { 0.0f, -5.0f, 0.0f };
	const float     FLOOR_W = 100.0f, FLOOR_H = 10.0f, FLOOR_D = 100.0f;
	auto			floor = Node::CreateNode<Node3D>("floor");
	auto			RB_floor = Node::CreateNode<NodeRigidBody>("rb_floor");
	auto			C_floor = Node::CreateNode<NodeBoxCollider>("c_floor");

	floor->SetLocalPosition({ 0.0f, -1.0f, 0.0f });
	PhysicsServer::FlushCommands(); 
	floor->AddChild(std::move(RB_floor));
	EngineServer::FlushCommands(); 

	auto ref_floorRB = static_cast<NodeRigidBody*>(&floor->GetChild(0));


	C_floor->SetShape({ 50.0f, 1.0f, 50.0f });
	ref_floorRB->SetBounciness(0.0f);

	//ref_floorRB->SetNode3DParent(floor.get());
	ref_floorRB->SetBodyType(RigidBodyType::STATIC);

	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider
	ref_floorRB->AddChild(std::move(C_floor));
	PhysicsServer::FlushCommands(); // Important pour que le RB soit cree avant d'ajouter le collider


	// params
	const float SPEED = 4.0f;
	const float ROT_SPEED = 90.0f;
	const float FIXED_DT = 1.0f / 60.0f;
	float accumulator = 0.0f;
	bool  isColliding = false;

	Mesh cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f);
	Material cubeMaterial = LoadMaterialDefault();

	EngineServer::FlushCommands(); // Important pour que les RB soient tous cree avant de faire le cast des colliders

	auto ref_playerCollider = static_cast<NodeBoxCollider*>(&ref_playerRB->GetChild(0));
	auto ref_triggerCollider = static_cast<NodeBoxCollider*>(&ref_triggerRB->GetChild(0));
	auto ref_wallCollider = static_cast<NodeBoxCollider*>(&ref_wallRB->GetChild(0));
	auto ref_floorCollider = static_cast<NodeBoxCollider*>(&ref_floorRB->GetChild(0));

	PhysicsServer::FlushCommands();

	// evrything below has to be done AFTER AddChild & EngineServer::FlushCommands() -- (because collider crerated when attached as RB child)
	player->AddScale({ 1.0f, 3.0f, 0.0f });
	ref_playerCollider->SetShape(player->GetScale());

	ref_triggerCollider->SetIsTrigger(true);
	ref_triggerCollider->SetMassDensity(1.0f);
	ref_triggerCollider->SetBounciness(0.0f);
	ref_floorCollider->SetBounciness(0.0f);

	// -- Collision filtering -- has to be done AFTER AddChild & EngineServer::FlushCommands() --
	ref_triggerCollider->SetFrictionCoefficient(1.0f);
	ref_playerCollider->SetCollisionCategoryBits(MASK_PLAYER);
	ref_playerCollider->SetCollideWithMaskBits(MASK_ENVIRONMENT);

	ref_triggerCollider->SetCollisionCategoryBits(MASK_TRIGGER);
	ref_triggerCollider->SetCollideWithMaskBits(MASK_PLAYER);

	ref_wallCollider->SetCollisionCategoryBits(MASK_ENVIRONMENT);
	//ref_wallCollider->SetCollideWithMaskBits(MASK_PLAYER);
	ref_wallCollider->SetCollideWithMaskBits(MASK_NONE);

	ref_floorCollider->SetCollisionCategoryBits(MASK_ENVIRONMENT);
	ref_floorCollider->SetCollideWithMaskBits(MASK_PLAYER);

	//// -- Joint test --
	//const rp3d::Vector3 anchorPointWorldSpace(5.0f, 10.0f, 0.0f);
	//rp3d::BallAndSocketJointInfo ballInfo(ref_playerRB->GetRigidBody(),ref_wallRB->GetRigidBody(), anchorPointWorldSpace);
	//ballInfo.isCollisionEnabled = false; // collisions between the two bodies connected by the joint 
	//rp3d::BallAndSocketJoint* joint;
	//joint = static_cast<rp3d::BallAndSocketJoint*>(PhysicsServer::GetPhysicsWorld().createJoint(ballInfo));

	// -- Event listener --
	//CollisionCB collisionCB;
	//OverlapCB overlapCB;
	//rp3d::EventListener* prevListener = PhysicsServer::GetPhysicsWorld().getEventListener();
	Events eventListener;
	PhysicsServer::GetPhysicsWorld().setEventListener(&eventListener);

	PhysicsServer::FlushCommands();
	bool test = false;
	bool trigg = ref_triggerCollider->IsTrigger();
	bool t = false;
	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		if (dt <= 0.0f || dt > 0.1f) dt = FIXED_DT;
																						// AZERTY KEAYBOARD
		if (IsKeyDown(KEY_W)) ref_playerRB->ApplyWorldForceAtCenterOfMass({ 0, 0, 500 });	// Forward = Z
		if (IsKeyDown(KEY_S)) ref_playerRB->ApplyWorldForceAtCenterOfMass({ 0, 0, -500 });	// Backward = S
		if (IsKeyDown(KEY_D)) ref_playerRB->ApplyWorldForceAtCenterOfMass({ -500, 0, 0 });	// Right = D
		if (IsKeyDown(KEY_A)) ref_playerRB->ApplyWorldForceAtCenterOfMass({ 500, 0, 0 });	// Left = Q

		if (IsKeyDown(KEY_Q)) ref_playerRB->ApplyWorldForceAtCenterOfMass({ 0, 500, 0 });	// Up = A
		if (IsKeyDown(KEY_E)) ref_playerRB->ApplyWorldForceAtCenterOfMass({ 0, -500, 0 });	// Down = E

		if (IsKeyDown(KEY_V))  ref_playerRB->ApplyLocalTorque({ 500,0,0 });
		if (IsKeyDown(KEY_B)) ref_playerRB->ApplyLocalTorque({ 0,500,0 });
		if (IsKeyDown(KEY_N))  ref_playerRB->ApplyLocalTorque({ 0,0,500 });

		if (IsKeyDown(KEY_C))  ref_playerRB->ResetTorque();

		if (IsKeyPressed(KEY_G))
		{
			test = !test;
			ref_playerRB->LockAngularAxis(test, test, test);
			std::string a = test == true ? "true" : "false";
			std::string b = "\n Linear lock = " + a + "\n";
			DEBUG(b);
		}
		if (IsKeyPressed(KEY_H))
		{
			t = !t;
			ref_playerRB->LockLinearAxis(t, t, t);
			std::string a = t == true ? "true" : "false";
			std::string b = "\n Angular lock = " + a + "\n";
			DEBUG(b);
		}
		if (IsKeyPressed(KEY_T))
		{
			trigg = !trigg;
			std::string a;
			if(trigg)
			{
				a = "Trigger = true\n";
			}
			else
			{
				a = "Trigger = false\n";
			}
			ref_triggerCollider->SetIsTrigger(trigg);
			DEBUG(a);
		}

		player->Update(dt);
		ref_playerRB->Update(dt);
		wall->Update(dt);
		ref_wallRB->Update(dt);
		PhysicsServer::FlushCommands();

		eventListener.Reset();

		// --- update physics ---
		accumulator += dt;
		while (accumulator >= FIXED_DT) {

			PhysicsServer::UpdatePhysicsWorld(FIXED_DT);
			accumulator -= FIXED_DT;
		}

		// --- test collision ---
		isColliding = eventListener.hit;


		// --- test overlap ---
		// GENERAL OVERLAP
		//if(PhysicsServer::GetPhysicsWorld().testOverlap(ref_playerRB->GetRigidBody(), ref_triggerRB->GetRigidBody()))
		//	DEBUG("Overlap test\n");

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

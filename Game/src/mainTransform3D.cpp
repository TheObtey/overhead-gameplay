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
#include "Clock.hpp"
#include <Debug.h>
//
//namespace rp = reactphysics3d;
//
//int main()
//{
//	std::cout << "main transform 3D" << std::endl;
//	// Initialization
//   //--------------------------------------------------------------------------------------
//	const int screenWidth = 1500;
//	const int screenHeight = 850;
//
//	InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");
//
//	// Define the camera to look into our 3d world
//	Camera3D camera = { 0 };
//	camera.position = Vector3{ 0.0f, 10.0f, 15.0f }; // Camera position
//	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
//	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
//	camera.fovy = 45.0f;                                // Camera field-of-view Y
//	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
//
//	Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
//	Material cubeMaterial = LoadMaterialDefault();
//
//	PhysicsServer::Init();
//
//	auto cube1 = Node::CreateNode<Node3D>("a");
//	auto rigidBody1 = Node::CreateNode<NodeRigidBody>("aRB");
//	cube1->SetPosition({ 0.0,5.0,0.0 });
//
//	auto cube2 = Node::CreateNode<Node3D>("b");
//	cube2->SetPosition({ 0.0,2.0,0.0 });
//	//cube2->SetScale({ 10.0,1.0,10.0});
//	auto rigidBody2 = Node::CreateNode<NodeRigidBody>("bRB");
//
//
//
//	// Create walls
//
//	auto wall1 = Node::CreateNode<Node3D>("wall1");
//	wall1->SetPosition({ 15.0,15.0,0.0 });
//	wall1->SetScale({ 1.0,30.0,30.0 });
//	auto rigidBody_wall1 = Node::CreateNode<NodeRigidBody>("rb_w1");
//
//	auto wall2 = Node::CreateNode<Node3D>("wall2");
//	wall2->SetPosition({ -15.0,15.0,0.0 });
//	wall2->SetScale({ 1.0,30.0,30.0 });
//	auto rigidBody_wall2 = Node::CreateNode<NodeRigidBody>("rb_w2");
//
//	auto wall3 = Node::CreateNode<Node3D>("wall3");
//	wall3->SetPosition({ 0.0,15.0,15.0 });
//	wall3->SetScale({ 30.0,30.0,1.0 });
//	auto rigidBody_wall3 = Node::CreateNode<NodeRigidBody>("rb_w3");
//
//	auto wall4 = Node::CreateNode<Node3D>("wall4");
//	wall4->SetPosition({ 0.0,15.0,-15.0 });
//	wall4->SetScale({ 30.0,30.0,1.0 });
//	auto rigidBody_wall4 = Node::CreateNode<NodeRigidBody>("rb_w4");
//
//
//	auto floor = Node::CreateNode<Node3D>("floor");
//	floor->SetPosition({ 0.0,-2.0,0.0 });
//	floor->SetScale({ 30.0,1.0,30.0 });
//	auto rigidBody_floor = Node::CreateNode<NodeRigidBody>("rb_floor");
//
//	auto ceil = Node::CreateNode<Node3D>("ceil");
//	ceil->SetPosition({ 0.0,30.0,0.0 });
//	ceil->SetScale({ 30.0,1.0,30.0 });
//	auto rigidBody_ceil = Node::CreateNode<NodeRigidBody>("rb_ceil");
//
//
//
//	cube1->Update(0.016f);
//	cube2->Update(0.016f);
//
//	//wall1->Update(0.016f);
//	//wall2->Update(0.016f);
//	//wall3->Update(0.016f);
//	//wall4->Update(0.016f);
//	floor->Update(0.016f);
//	ceil->Update(0.016f);
//
//	rigidBody1->Init(cube1.get());
//	rigidBody1->AddBoxCollider({2.0,2.0,2.0});
//	rigidBody2->Init(cube2.get());
//	//rigidBody2->AddBoxCollider({ 10.0,1.0,10.0 });
//	rigidBody2->AddBoxCollider({ 1.0,1.0,1.0 });
//
//	//rigidBody2->GetRigidBody().setTransform(*cube2.get());
//	//auto tr = rigidBody2.get()->GetRigidBody().getTransform();
//	rigidBody1->GetRigidBody().enableGravity(false);
//	rigidBody2->GetRigidBody().enableGravity(false);
//
//	rigidBody1->GetRigidBody().setType(rp::BodyType::DYNAMIC);
//	rigidBody1->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//	rigidBody2->GetRigidBody().setType(rp::BodyType::STATIC);
//	rigidBody2->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//
//
//	//rigidBody_wall1->Init(cube1.get());
//	//rigidBody_wall1->AddBoxCollider({ 1.0,30.0,30.0 });
//	//rigidBody_wall1->GetRigidBody().setType(rp::BodyType::STATIC);
//	//rigidBody_wall1->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//
//	//rigidBody_wall2->Init(wall2.get());
//	//rigidBody_wall2->AddBoxCollider({ 1.0,30.0,30.0 });
//	//rigidBody_wall2->GetRigidBody().setType(rp::BodyType::STATIC);
//	//rigidBody_wall2->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//
//	//rigidBody_wall3->Init(wall3.get());
//	//rigidBody_wall3->AddBoxCollider({ 30.0,30.0,1.0 });
//	//rigidBody_wall3->GetRigidBody().setType(rp::BodyType::STATIC);
//	//rigidBody_wall3->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//
//	//rigidBody_wall4->Init(wall4.get());
//	//rigidBody_wall4->AddBoxCollider({ 30.0,30.0,1.0 });
//	//rigidBody_wall4->GetRigidBody().setType(rp::BodyType::STATIC);
//	//rigidBody_wall4->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//
//
//	//rigidBody_ceil->Init(ceil.get());
//	//rigidBody_ceil->AddBoxCollider({ 30.0,1.0,30.0 });
//	//rigidBody_ceil->GetRigidBody().setType(rp::BodyType::STATIC);
//	//rigidBody_ceil->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//
//	rigidBody_floor->Init(floor.get());
//	rigidBody_floor->AddBoxCollider({ 10.0,2.0,10.0 });
//	rigidBody_floor->GetRigidBody().setType(rp::BodyType::STATIC);
//	rigidBody_floor->GetRigidBody().getCollider(0)->setIsSimulationCollider(true);
//
//
//	//EngineServer::FlushCommands();
//
//
//	Node3D& cube1ref = *cube1.get();
//	Node3D& cube2ref = *cube2.get();
//	// Add children
//	//EngineServer::FlushCommands();
//
//
//
//	//PhysicsServer::FlushCommands();
//
//	DisableCursor();                    // Limit cursor to relative movement inside the window
//
//	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
//	EnableCursor();
//	//--------------------------------------------------------------------------------------
//
//	// PHYSICS
//
//	//rp::PhysicsCommon physicsCommon;
//
//	//// Create the world settings
//	//rp::PhysicsWorld::WorldSettings settings;
//	//settings.defaultVelocitySolverNbIterations = 15;
//	//settings.defaultPositionSolverNbIterations = 10;
//	//settings.isSleepingEnabled = true;
//	//settings.gravity = rp::Vector3(0, -9.81, 0);
//	//// Create a physics world
//	//rp::PhysicsWorld* world = physicsCommon.createPhysicsWorld();
//
//	//rp::RigidBody* body = world->createRigidBody(*cube1.get());
//
//	//--------------------------------------------------------------------------------------
//
//
//	const rp::decimal timeStep = 1.0f / 60.0f;
//
//	// Main game loop
//	while (!WindowShouldClose())        // Detect window close button or ESC key
//	{
//		// Events
//		//if (IsKeyPressed(KEY_Z)) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
//
//		//// move cube
//		//if (IsKeyDown(KEY_I)) cube1ref.AddPosition({ 0.0, 0.0, 0.10 });
//		//if (IsKeyDown(KEY_K)) cube1ref.AddPosition({ 0.0, 0.0, -0.10 });
//		//if (IsKeyDown(KEY_J)) cube1ref.AddPosition({ -0.10, 0.0, 0.0 });
//		//if (IsKeyDown(KEY_L)) cube1ref.AddPosition({ 0.10, 0.0, 0.0 });
//		////if (IsKeyDown(KEY_G)) cube2ref.AddPosition({ 1.0, 0.0, 0.0 });
//		//if (IsKeyDown(KEY_O)) cube1ref.AddPosition({ 0,0.1,0 });
//		//if (IsKeyDown(KEY_U)) cube1ref.AddPosition({ 0,-0.1,0 });
//
//		if (IsKeyDown(KEY_U)) rigidBody1->GetRigidBody().applyWorldForceAtCenterOfMass({ 0,100,0 });
//		if (IsKeyDown(KEY_O)) rigidBody1->GetRigidBody().applyWorldForceAtCenterOfMass({ 0,-100,0 });
//		if (IsKeyDown(KEY_I)) rigidBody1->GetRigidBody().applyWorldForceAtCenterOfMass({ 0,0,-100 });
//		if (IsKeyDown(KEY_K)) rigidBody1->GetRigidBody().applyWorldForceAtCenterOfMass({ 0,0,100 });
//		if (IsKeyDown(KEY_J)) rigidBody1->GetRigidBody().applyWorldForceAtCenterOfMass({ -100,0,0 });
//		if (IsKeyDown(KEY_L)) rigidBody1->GetRigidBody().applyWorldForceAtCenterOfMass({ 100,0,0 });
//
//		if (IsKeyDown(KEY_RIGHT)) cube2ref.AddPosition({ 1.0, 0.0, 0.0 });
//		if (IsKeyDown(KEY_UP)) cube2ref.AddRoll(0.2);
//
//		if (IsKeyDown(KEY_N)) cube1ref.AddYaw(0.2);
//		if (IsKeyDown(KEY_B)) cube1ref.AddPitch(0.2);
//		if (IsKeyDown(KEY_V)) cube1ref.AddRoll(0.2);
//
//		if (IsKeyPressed(KEY_C)) cube1ref.AddScale({ 0.0, 1.0, 0.0 });
//		if (IsKeyDown(KEY_X)) cube1ref.AddScale({ -1.0, -1.0, -1.0 });
//
//		if (IsKeyPressed(KEY_Y))
//		{
//			//cube4ref.Reparent(cube1ref.GetChild(0));
//			auto newCube = Node::CreateNode<Node3D>("newCube");
//
//		}
//
//		// Update
//		//----------------------------------------------------------------------------------
//		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
//			UpdateCamera(&camera, CAMERA_FREE);
//
//		/*if (rigidBody2->GetRigidBody().testPointInside(Vec3(cube1->GetPosition().x, cube1->GetPosition().y, cube1->GetPosition().z)))
//			DEBUG("\nPOINT IN CUBE 2\n");*/
//
//		cube1->Update(0.016f);
//		cube2->Update(0.016f);
//
//		//wall1->Update(0.016f);
//		//wall2->Update(0.016f);
//		//wall3->Update(0.016f);
//		//wall4->Update(0.016f);
//		//floor->Update(0.016f);
//		//ceil->Update(0.016f);
//
//		//rigidBody_wall1->Update(0.016f);
//		//rigidBody_wall2->Update(0.016f);
//		//rigidBody_wall3->Update(0.016f);
//		//rigidBody_wall4->Update(0.016f);
//		//rigidBody_ceil->Update(0.016f);
//		rigidBody_floor->Update(0.016f);
//		rigidBody1->Update(0.016f);
//		rigidBody2->Update(0.016f);
//
//
//		//----------------------------------------------------------------------------------
//
//		// Draw
//		//----------------------------------------------------------------------------------
//		BeginDrawing();
//
//		ClearBackground(RAYWHITE);
//
//		BeginMode3D(camera);
//
//		PhysicsServer::GetPhysicsWorld().update(timeStep);
//		//world->update(timeStep);
//
//
//		glm::mat4 m1 = cube1->GetWorldMatrix();
//		glm::mat4 m2 = cube2ref.GetWorldMatrix();
//		//glm::mat4 m3 = cube3ref.GetWorldMatrix();
//		//glm::mat4 m4 = cube4ref.GetWorldMatrix();
//
//		// Walls
//		glm::mat4 mWall1 = wall1->GetWorldMatrix();
//		glm::mat4 mWall2 = wall2->GetWorldMatrix();
//		glm::mat4 mWall3 = wall3->GetWorldMatrix();
//		glm::mat4 mWall4 = wall4->GetWorldMatrix();
//
//		glm::mat4 mCeil = ceil->GetWorldMatrix();
//		glm::mat4 mFloor = floor->GetWorldMatrix();
//
//		auto getMat = [](glm::mat4 const& m) -> Matrix
//			{
//				return {
//					m[0][0], m[1][0], m[2][0], m[3][0],
//					m[0][1], m[1][1], m[2][1], m[3][1],
//					m[0][2], m[1][2], m[2][2], m[3][2],
//					m[0][3], m[1][3], m[2][3], m[3][3]
//				};
//			};
//
//		Matrix rlMat1 = getMat(m1);
//		Matrix rlMat2 = getMat(m2);
//
//		Matrix wallMat1 = getMat(mWall1);
//		Matrix wallMat2 = getMat(mWall2);
//		Matrix wallMat3 = getMat(mWall3);
//		Matrix wallMat4 = getMat(mWall4);
//
//		Matrix ceilMat = getMat(mCeil);
//		Matrix floorMat = getMat(mFloor);
//
//		//Matrix rlMat1 = {
//		//	m1[0][0], m1[1][0], m1[2][0], m1[3][0],
//		//	m1[0][1], m1[1][1], m1[2][1], m1[3][1],
//		//	m1[0][2], m1[1][2], m1[2][2], m1[3][2],
//		//	m1[0][3], m1[1][3], m1[2][3], m1[3][3]
//		//};
//
//		//Matrix rlMat2 = {
//		//	m2[0][0], m2[1][0], m2[2][0], m2[3][0],
//		//	m2[0][1], m2[1][1], m2[2][1], m2[3][1],
//		//	m2[0][2], m2[1][2], m2[2][2], m2[3][2],
//		//	m2[0][3], m2[1][3], m2[2][3], m2[3][3]
//		//};
//
//		//Matrix rlMat3 = {
//		//	m3[0][0], m3[1][0], m3[2][0], m3[3][0],
//		//	m3[0][1], m3[1][1], m3[2][1], m3[3][1],
//		//	m3[0][2], m3[1][2], m3[2][2], m3[3][2],
//		//	m3[0][3], m3[1][3], m3[2][3], m3[3][3]
//		//};
//
//		//Matrix rlMat4 = {
//		//	m4[0][0], m4[1][0], m4[2][0], m4[3][0],
//		//	m4[0][1], m4[1][1], m4[2][1], m4[3][1],
//		//	m4[0][2], m4[1][2], m4[2][2], m4[3][2],
//		//	m4[0][3], m4[1][3], m4[2][3], m4[3][3]
//		//};
//
//		auto drawWall = [&](Matrix const& mat)
//			{
//				cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = BEIGE;
//				DrawMesh(cubeMesh, cubeMaterial, mat);
//			};
//
//		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = RED;
//		DrawMesh(cubeMesh, cubeMaterial, rlMat1);
//
//		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = VIOLET;
//		DrawMesh(cubeMesh, cubeMaterial, rlMat2);
//
//		//drawWall(wallMat1);
//		//drawWall(wallMat2);
//		//drawWall(wallMat3);
//		//drawWall(wallMat4);
//
//		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
//		DrawMesh(cubeMesh, cubeMaterial, ceilMat);
//		cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
//		DrawMesh(cubeMesh, cubeMaterial, floorMat);
//
//		//cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
//		//DrawMesh(cubeMesh, cubeMaterial, rlMat3);
//
//		//cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
//		//DrawMesh(cubeMesh, cubeMaterial, rlMat4);
//
//
//		DrawGrid(10, 1.0f);
//
//		EndMode3D();
//
//		DrawRectangle(10, 10, 320, 93, Fade(SKYBLUE, 0.5f));
//		DrawRectangleLines(10, 10, 320, 93, BLUE);
//
//		EndDrawing();
//		//----------------------------------------------------------------------------------
//	}
//
//	// De-Initialization
//	//--------------------------------------------------------------------------------------
//	CloseWindow();        // Close window and OpenGL context
//	//--------------------------------------------------------------------------------------
//
//
//
//}


#include "raymath.h"
#include "rlgl.h"

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

int main() {
    InitWindow(1280, 720, "Cube");
    SetTargetFPS(60);

    Camera3D cam = {};
    cam.position = { 0.0f, 10.0f, -20.0f };
    cam.target = { 0.0f,  0.0f,  0.0f };
    cam.up = { 0.0f,  1.0f,  0.0f };
    cam.fovy = 50.0f;
    cam.projection = CAMERA_PERSPECTIVE;

	////// Create physics world
 //   rp3d::PhysicsCommon physicsCommon;
 //   rp3d::PhysicsWorld::WorldSettings ws;
	//ws.defaultBounciness = 0.0f;
	//ws.defaultFrictionCoefficient = 0.5f;
 //   ws.defaultVelocitySolverNbIterations = 3000;
 //   ws.defaultPositionSolverNbIterations = 1500;
 //   ws.isSleepingEnabled = true;    
 //   ws.gravity = { 0, -19.81, 0 };
 //   rp3d::PhysicsWorld* world = physicsCommon.createPhysicsWorld(ws);

	PhysicsServer::Init();

    // cube
    Vector3 pos = { 0.0f, 1.0f, 0.0f };
    Vector3 pSize = { 1.0f, 1.0f, 1.0f };
    float   rotY = 0.0f;   
    float   rotX = 0.0f;   

    auto RB_player = Node::CreateNode<NodeRigidBody>("rb_player");
    auto player = Node::CreateNode<Node3D>("player");
    player.get()->SetPosition({ pos.x,pos.y,pos.z });
    //player->Update(0.016);
    RB_player->Init(player.get());
    RB_player->GetRigidBody().setType(rp3d::BodyType::DYNAMIC);
    RB_player->AddBoxCollider({ pSize.x, pSize.y, pSize.z });
    RB_player->GetRigidBody().enableGravity(false);
    RB_player->GetRigidBody().setMass(1.0f);

    //rp3d::RigidBody* playerBody = world->createRigidBody(
    //    rp3d::Transform(rp3d::Vector3(pos.x, pos.y, pos.z),
    //        rp3d::Quaternion::identity()));
    //playerBody->setType(rp3d::BodyType::DYNAMIC);
    //playerBody->addCollider(
    //    physicsCommon.createBoxShape(rp3d::Vector3(1.0f, 1.0f, 1.0f)),
    //    rp3d::Transform::identity());
	//playerBody->enableGravity(false);
	//playerBody->setMass(1.0f);

    //  Mur statique 
    const Vector3   WALL_POS = { 3.0f, 1.0f, 0.0f };
    const float     WALL_W = 1.0f;   
    const float     WALL_H = 4.0f;   
    const float     WALL_D = 6.0f;   

    auto RB_wall = Node::CreateNode<NodeRigidBody>("rb_wall");
    auto wall = Node::CreateNode<Node3D>("wall");
    wall.get()->SetPosition({ WALL_POS.x,WALL_POS.y,WALL_POS.z });
    RB_wall->Init(wall.get());
    RB_wall->GetRigidBody().setType(rp3d::BodyType::STATIC);
    RB_wall->AddBoxCollider(Vec3(WALL_W / 2, WALL_H / 2, WALL_D / 2));
    //RB_wall->GetRigidBody().enableGravity(false);
    //RB_wall->GetRigidBody().setMass(1.0f);

   /* rp3d::RigidBody* wallBody = world->createRigidBody(
        rp3d::Transform(rp3d::Vector3(WALL_POS.x, WALL_POS.y, WALL_POS.z),
            rp3d::Quaternion::identity()));
    wallBody->setType(rp3d::BodyType::STATIC);
    wallBody->addCollider(
        physicsCommon.createBoxShape(rp3d::Vector3(WALL_W / 2, WALL_H / 2, WALL_D / 2)),
        rp3d::Transform::identity());*/

    //  Sol statique 
    const Vector3   FLOOR_POS = { 0.0f, -1.0f, 0.0f };
    const float     FLOOR_W = 100.0f;   
    const float     FLOOR_H = 2.0f;   
    const float     FLOOR_D = 100.0f;   

    auto RB_floor = Node::CreateNode<NodeRigidBody>("rb_floor");
    auto floor = Node::CreateNode<Node3D>("floor");
    floor.get()->SetPosition({ FLOOR_POS.x,FLOOR_POS.y,FLOOR_POS.z });
    RB_floor->Init(floor.get());
    RB_floor->GetRigidBody().setType(rp3d::BodyType::STATIC);
    RB_floor->AddBoxCollider(Vec3(FLOOR_W / 2, FLOOR_H / 2, FLOOR_D / 2));

    //rp3d::RigidBody* floorBody = world->createRigidBody(
    //    rp3d::Transform(rp3d::Vector3(FLOOR_POS.x, FLOOR_POS.y, FLOOR_POS.z),
    //        rp3d::Quaternion::identity()));
    //floorBody->setType(rp3d::BodyType::STATIC);
    //floorBody->addCollider(
    //    physicsCommon.createBoxShape(rp3d::Vector3(FLOOR_W / 2, FLOOR_H / 2, FLOOR_D / 2)),
    //    rp3d::Transform::identity());

    // params
    const float SPEED = 4.0f;
    const float ROT_SPEED = 90.0f;
    const float FIXED_DT = 1.0f / 60.0f;
    float accumulator = 0.0f;
    bool  isColliding = false;

    //RB_player->GetRigidBody().setAngularLockAxisFactor(rp3d::Vector3(0.9, 0.9, 0.9));
    //playerBody->setLinearLockAxisFactor
    //RB_player->GetRigidBody().setLinearDamping(8.0f);
    //playerBody->setAngularDamping(8.0f);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt <= 0.0f || dt > 0.1f) dt = FIXED_DT;

        // --- move  ---
        //rp3d::Vector3 currentVel = playerBody->getLinearVelocity();
        rp3d::Vector3 currentVel = RB_wall->GetRigidBody().getLinearVelocity();

        rp3d::Vector3 input(0, 0, 0);
        if (IsKeyDown(KEY_W)) input.z += SPEED;
        if (IsKeyDown(KEY_S)) input.z -= SPEED;
        if (IsKeyDown(KEY_D)) input.x -= SPEED;
        if (IsKeyDown(KEY_A)) input.x += SPEED;
        if (IsKeyDown(KEY_E)) input.y += SPEED;  
        if (IsKeyDown(KEY_Q)) input.y -= SPEED;

        
        //rp3d::Vector3 newVel(
        //    input.x != 0 ? input.x : currentVel.x,  
        //    input.y != 0 ? input.y : currentVel.y,           
        //    input.z != 0 ? input.z : currentVel.z 
        //);
        rp3d::Vector3 newVel(
            input.x + currentVel.x,  
            input.y + currentVel.y,           
            input.z + currentVel.z 
        );

        //playerBody->setLinearVelocity(newVel);
        RB_player->GetRigidBody().setLinearVelocity(newVel);

        //auto rotQuat = playerBody->getTransform().getOrientation();
        auto rotQuat = RB_player->GetRigidBody().getTransform().getOrientation();
        //auto aVel = playerBody->getAngularVelocity();
        auto aVel = RB_player->GetRigidBody().getAngularVelocity();
        // --- rotate ---
        if (IsKeyDown(KEY_LEFT))  aVel.x -= ROT_SPEED * dt;
        if (IsKeyDown(KEY_RIGHT)) aVel.x += ROT_SPEED * dt;
        if (IsKeyDown(KEY_DOWN))  aVel.y += ROT_SPEED * dt;
        if (IsKeyDown(KEY_UP))    aVel.y -= ROT_SPEED * dt;

        //playerBody->setAngularVelocity(aVel);
        RB_player->GetRigidBody().setAngularVelocity(aVel);

        //playerBody->applyLocalForceAtLocalPosition({ 0.0,0.0,10.0 }, { 10.0,0.0,0.0 });
        //playerBody->applyLocalForceAtLocalPosition({ 0.0,0.0,10.0 }, {  0.0,0.0,10.0 }); // yaw
        //playerBody->applyLocalTorque({ 0.0,1000.0,100.0 });
        //playerBody->applyLocalForceAtLocalPosition({ 0.0,0.0,-1.0 }, { 1.0,0.0,0.0 }); // yaw
                       

		// --- update physics ---
        accumulator += dt;
        while (accumulator >= FIXED_DT) {
            //world->update(FIXED_DT);
            PhysicsServer::GetPhysicsWorld().update(FIXED_DT);
            accumulator -= FIXED_DT;
        }
        RB_player->Update(dt);
        player->Update(dt);

        RB_wall->Update(dt);
        wall->Update(dt);
        RB_floor->Update(dt);
        floor->Update(dt);
        
        //rp3d::Vector3 p = playerBody->getTransform().getPosition();
        rp3d::Vector3 p = RB_player->GetRigidBody().getTransform().getPosition();
        pos = { p.x, p.y, p.z };

        // --- test overlap ---
        OverlapCB cb;
        //world->testOverlap(playerBody, cb);
        PhysicsServer::GetPhysicsWorld().testOverlap(&RB_player->GetRigidBody(), cb);
        isColliding = cb.hit;

        // --- draw ---
        BeginDrawing();
        ClearBackground({ 20, 20, 30, 255 });

        BeginMode3D(cam);
        DrawGrid(30, 1.0f);
    
        auto draw = [&](Vector3 p, rp3d::Quaternion q, Color c) {
            // Convertir le quaternion rp3d en matrice column-major pour rlgl
            float mat[16] = {
                1 - 2 * (q.y * q.y + q.z * q.z),  2 * (q.x * q.y + q.w * q.z),      2 * (q.x * q.z - q.w * q.y),      0,
                2 * (q.x * q.y - q.w * q.z),      1 - 2 * (q.x * q.x + q.z * q.z),  2 * (q.y * q.z + q.w * q.x),      0,
                2 * (q.x * q.z + q.w * q.y),      2 * (q.y * q.z - q.w * q.x),      1 - 2 * (q.x * q.x + q.y * q.y),  0,
                p.x,                          p.y,                          p.z,                          1
            };
            rlPushMatrix();
            rlMultMatrixf(mat);
            Color cubeColor = isColliding ? Color{ 255, 60, 60, 255 } : c;
            DrawCube({ 0,0,0 }, 2.0f, 2.0f, 2.0f, cubeColor);
            DrawCubeWires({ 0,0,0 }, 2.0f, 2.0f, 2.0f, DARKBLUE);
            rlPopMatrix();
            };


        // Cube joueur
        //draw(pos,playerBody->getTransform().getOrientation(), {80, 160, 255, 255});
        draw(pos, RB_player->GetRigidBody().getTransform().getOrientation(), {80, 160, 255, 255});


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

        DrawText(TextFormat("RB_Pos: (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z),
            10, GetScreenHeight() - 48, 16, BLACK);
        DrawText(TextFormat("PlayerPos: (%.1f, %.1f, %.1f)", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z),
            10, GetScreenHeight() - 26, 16, BLACK);
        //DrawText(TextFormat("Rot: Y=%.1f  X=%.1f", rotY, rotX),
        //    10, GetScreenHeight() - 26, 16, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

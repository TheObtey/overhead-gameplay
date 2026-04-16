////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include <AssetLoading/AssetLoader.h>
#include <Window.h>
#include <Program.h>
#include <Shader.h>
#include <Passes/GeometryPass.h>
#include <Passes/LightPass.h>
#include <Passes/AnimatedPass.h>
#include <Logger.hpp>
#include <Nodes/NodeMeshAnimated3D.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeCollider.h>
#include <Nodes/NodeCamera.h>
#include <Nodes/NodeWindow.h>
#include <Nodes/NodeViewport.h>
#include <Servers/AnimationServer.h>
#include <Servers/GraphicServer.h>
#include <Servers/EngineServer.h>
#include <Servers/PhysicsServer.h>
#include <GameLoop.h>
#include <algorithm>

namespace rl
{
	#include "raylib.h"
}

uptr<Node> LoadScene()
{

    uptr<Node> scene = Node::CreateNode<Node>("Scene");

    uptr<Node> nViewport = Node::CreateNode<NodeViewport>("Viewport");

    //sptr<SceneData> Scene1 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim_3.fbx", AssetLoader::FileType::FBX);
    sptr<SceneData> Scene4 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim_2.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene2 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene3 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Bones.fbx", AssetLoader::FileType::FBX);

    uptr<Node> nCamera = Node::CreateNode<NodeCamera>("Camera");
    static_cast<NodeCamera*>(nCamera.get())->SetLocalZ(5);

    uptr<Node> pNode = Node::CreateNode<NodeMeshAnimated3D>("MeshAnimated");
    NodeMeshAnimated3D* pMesh = dynamic_cast<NodeMeshAnimated3D*>(pNode.get());
    pMesh->SetMesh(*Scene4->allMesh[0]);
    pMesh->SetAnimation(*Scene4->animations[0]);
    pMesh->SetLocalPosition({ 0.0f,0.0f,0.0f });

    nViewport->AddChild(nCamera);
    nViewport->AddChild(pNode);

    scene->AddChild(nViewport);

    return scene;
}
int main()
{
    uptr<NodeBoxCollider> b = Node::CreateNode<NodeBoxCollider>("aa");
    b->SetShape({ 0.10f,1.0f,1.0f });
    PhysicsServer::FlushCommands();

    uptr<Node> root = Node::CreateNode<NodeWindow>("Window");
    SceneTree sTree(root);

    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(sTree);
}
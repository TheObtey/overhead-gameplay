////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include <AssetLoading/AssetLoader.h>
#include <Window.h>
#include <Program.h>
#include <Shader.h>
#include <Geometry.h>
#include <Passes/GeometryPass.h>
#include <Passes/LightPass.h>
#include <Passes/AnimatedPass.h>
#include <Logger.hpp>
#include <Nodes/NodeMesh.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeCollider.h>
#include <Nodes/NodeCamera.h>
#include <Nodes/NodeWindow.h>
#include <Nodes/NodeViewport.h>
#include <Servers/GraphicServer.h>
#include <Servers/EngineServer.h>
#include <GameLoop.h>
#include <algorithm>
#include <EditorSerializer.h>


Node& LoadScene(Node& node)
{

    uptr<Node> scene = Node::CreateNode<Node>("Scene");

    uptr<Node> nViewport = Node::CreateNode<NodeViewport>("Viewport");

    uptr<Node> nCamera = Node::CreateNode<NodeCamera>("Camera");
    static_cast<NodeCamera*>(nCamera.get())->SetLocalPosition({0.0,20,0.0});

    //sptr<SceneData> Scene1 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim_3.fbx", AssetLoader::FileType::FBX);
    sptr<SceneData> Scene4 = AssetLoader::LoadSceneFromFile("res/Assets/Test_staff_room.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene2 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene3 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Bones.fbx", AssetLoader::FileType::FBX);

    uptr<Node> pNode = AssetLoader::CreateNodesFromScene(*Scene4);

    EditorSerializer::SaveNode("HubScene.nd",pNode);

    nViewport->AddChild(nCamera);
    nViewport->AddChild(pNode);

    node.AddChild(nViewport);

    return node;
}
int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Window");
    SceneTree sTree(root);

    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(sTree);
}
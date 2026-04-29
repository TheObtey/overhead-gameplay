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
    static_cast<NodeCamera*>(nCamera.get())->SetLocalPosition({0.0,0.0f,-5.0f});
    static_cast<NodeCamera*>(nCamera.get())->AddLocalYaw(1.0f);

    //sptr<SceneData> Scene1 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim_3.fbx", AssetLoader::FileType::FBX);
    sptr<SceneData> Scene1 = AssetLoader::LoadSceneFromFile("res/fbx/Player/BibHopeBone.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene4 = AssetLoader::LoadSceneFromFile("res/Assets/Staff_Room_Tutoriel/Staff_Room_Movable_Meshs.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene2 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene3 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Bones.fbx", AssetLoader::FileType::FBX);

    uptr<Node> pNode1 = AssetLoader::CreateNodesFromScene(*Scene1);
    //uptr<Node> pNode4 = AssetLoader::CreateNodesFromScene(*Scene4);
    static_cast<Node3D*>(pNode1.get())->SetLocalPosition({ 0.0f,0.0f,0.f });
    //static_cast<Node3D*>(pNode4.get())->SetLocalPosition({ 0.0f,0.0f,0.f });
    EditorSerializer::SaveNode("res/BibBones.nd", pNode1);
    //EditorSerializer::SaveNode("Staff_Room_Movable_Meshs.nd",pNode4);

    nViewport->AddChild(nCamera);
    nViewport->AddChild(pNode1);
    //nViewport->AddChild(pNode4);

    node.AddChild(nViewport);

    return node;
}
int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Window");
    SceneTree sTree(root);
    std::cout << "je suis main asset loading" << std::endl;
    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(sTree);
}
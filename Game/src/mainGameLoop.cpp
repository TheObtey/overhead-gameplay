#include "GameLoop.h"
#include "SceneTree.h"
#include "Scripting/Proxies/ActionMapProxy.h"
#include "Scripting/Proxies/MathsProxy.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "Nodes/NodeWindow.h"
#include "Servers/EngineServer.h"

#include "Scripting/Lua/LuaScriptInstance.hpp"

using namespace Ore;

uptr<Node> LoadScene()
{
    sptr<Texture> normal = std::make_shared<Texture>("res/textures/NormalMap.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL);
    sptr<Texture> specular = std::make_shared<Texture>("res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);

    uptr<Node> scene = Node::CreateNode<Node>("Scene");

    uptr<Node> viewport = Node::CreateNode<NodeViewport>("Viewport");
    //uptr<Node> viewport2 = Node::CreateNode<NodeViewport>("Viewport2");

    uptr<NodeCamera> camera = Node::CreateNode<NodeCamera>("Camera");
    camera->SetLocalZ(5);

    //uptr<NodeCamera> camera2 = Node::CreateNode<NodeCamera>("Camera2");
    //camera2->SetLocalZ(5);

    uptr<Node3D> node3d = Node::CreateNode<Node3D>("node3d");

    uptr<NodeMesh> mesh = Node::CreateNode<NodeMesh>("Cube");
    mesh->AddTextures(normal, specular);
    //mesh->SetWorldPosition({ 0.0,1.5,0.0 });
	mesh->SetLocalX(2);
    //mesh->SetWorldPosition({ 2, 0, 0});

    uptr<LuaScriptInstance> scriptNode3D = std::make_unique<LuaScriptInstance>("res/scripts/TestNode3D.lua");
    Node::AttachScript(scriptNode3D, *mesh);

    //uptr<NodeMesh> mesh2 = Node::CreateNode<NodeMesh>("Cube2");
    //mesh2->AddTextures(normal, specular);
    //mesh2->SetLocalX(-2);

    //uptr<Node> loadedNode = Node::LoadNodeFromJSON<Node>("res/scripts/prefabs/TestNode.nd.json");

    viewport->AddChild(std::move(camera));
    viewport->AddChild(std::move(mesh));

    //viewport2->AddChild(std::move(mesh2));
    //viewport2->AddChild(std::move(camera2));

    scene->AddChild(std::move(viewport));
    //scene->AddChild(std::move(viewport2));

    uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>("res/scripts/test.lua");
    Node::AttachScript(script, *scene);

	
    std::cout << "Script attached to node3d" << std::endl;

    return scene;
}

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Window");

    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(defaultSceneTree);
}

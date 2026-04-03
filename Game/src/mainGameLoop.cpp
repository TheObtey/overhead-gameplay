#include "GameLoop.h"
#include "SceneTree.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "Nodes/NodeWindow.h"

#include "Scripting/Lua/LuaScriptInstance.hpp"

uptr<Node> LoadScene()
{
    sptr<Texture> normal = std::make_shared<Texture>("res/textures/NormalMap.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL);
    sptr<Texture> specular = std::make_shared<Texture>("res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);

    uptr<Node> scene = Node::CreateNode<Node>("Scene");

    uptr<Node> viewport = Node::CreateNode<NodeViewport>("Viewport");
    uptr<Node> viewport2 = Node::CreateNode<NodeViewport>("Viewport2");

    uptr<NodeCamera> camera = Node::CreateNode<NodeCamera>("Camera");
    camera->SetLocalZ(5);

    uptr<NodeCamera> camera2 = Node::CreateNode<NodeCamera>("Camera2");
    camera2->SetLocalZ(5);

    uptr<NodeMesh> mesh = Node::CreateNode<NodeMesh>("Cube");
    mesh->AddTextures(normal, specular);
    mesh->SetLocalX(1.5);

    uptr<NodeMesh> mesh2 = Node::CreateNode<NodeMesh>("Cube2");
    mesh2->AddTextures(normal, specular);
    //mesh2->SetLocalX(-2);

    viewport2->AddChild(std::move(camera));
    viewport2->AddChild(std::move(mesh));

    viewport->AddChild(std::move(mesh2));
    viewport->AddChild(std::move(camera2));

    scene->AddChild(std::move(viewport));
    scene->AddChild(std::move(viewport2));

    uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>("res/test.lua");
    Node::AttachScript(script, *scene);

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

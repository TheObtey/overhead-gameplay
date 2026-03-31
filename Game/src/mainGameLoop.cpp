#include "GameLoop.h"
#include "SceneTree.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "Nodes/NodeWindow.h"

uptr<Node> LoadScene()
{
    uptr<Node> scene = Node::CreateNode<Node>("Scene");
    uptr<NodeCamera> camera = Node::CreateNode<NodeCamera>("Camera");
    camera->SetLocalZ(5);
    camera->SetLocalY(-90);

    uptr<Node> mesh = Node::CreateNode<NodeMesh>("Cube");
    scene->AddChild(std::move(camera));
    scene->AddChild(mesh);

    return scene;
}

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Window");
    uptr<Node> viewport = Node::CreateNode<NodeViewport>("Viewport");
    root->AddChild(viewport);

    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(defaultSceneTree);
}

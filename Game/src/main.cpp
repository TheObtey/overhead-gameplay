#include "GameLoop.h"
#include "SceneTree.h"
#include "Scripting/Proxies/MathsProxy.h"
#include "Nodes/NodeViewport.h"
#include "Nodes/NodeWindow.h"
#include "Servers/EngineServer.h"

Node& LoadScene(Node& root)
{
    uptr<NodeViewport> v = Node::CreateNode<NodeViewport>("MainViewport");
    Node* viewport = v.get();
    root.AddChild(std::move(v));

    uptr<Node> scene = Node::LoadNodeFromJSON<Node>("res/scenes/default.sc.json");
    viewport->AddChild(std::move(scene));
    return *viewport;
}

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Window");
    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(defaultSceneTree);
}

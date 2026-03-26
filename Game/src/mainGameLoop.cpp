#include "GameLoop.h"
#include "SceneTree.h"
#include "Nodes/NodeWindow.h"

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Root");
    uptr<Node> scene = Node::CreateNode<Node>("Scene");    //Load this with the default .st file
    root->AddChild(scene);
    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.StartGame(defaultSceneTree);
}

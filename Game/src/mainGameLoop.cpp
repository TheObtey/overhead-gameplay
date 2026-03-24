#include "GameLoop.h"
#include "SceneTree.h"
#include "Nodes/NodeWindow.h"

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Root");
    uptr<Node> scene = Node::CreateNode<Node>("Scene");    //Load this with the default .st file
    uptr<Node> test = Node::CreateNode<Node>("test");    //Load this with the default .st file
    scene->AddChild(test);
    root->AddChild(scene);
    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.StartGame(defaultSceneTree);
}

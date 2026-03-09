#include "GameLoop.h"
#include "SceneTree.h"

int main()
{
    //Load this with the default .st file
    uptr<Node> root = Node::CreateNode<Node>("Root");
    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.StartGame(defaultSceneTree);
}

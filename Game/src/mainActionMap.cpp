
#include "EventManager.h"
#include "ActionMap.h"
#include "Action.h"

#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include "GameLoop.h"
#include "SceneTree.h"
#include <Nodes/NodeWindow.h>

#include "Scripting/Lua/LuaScriptInstance.hpp"

uptr<Node> LoadScene()
{
    uptr<Node> scene = Node::CreateNode<Node>("Scene");

    return scene;
}

int main(int argc, char** argv)
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Root");
    uptr<Node> scene = Node::CreateNode<Node>("Scene");    //Load this with the default .st file

    uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>("res/test.lua");
    Node::AttachScript(script, *scene);
    root->AddChild(scene);

    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(defaultSceneTree);


	return 0;
}

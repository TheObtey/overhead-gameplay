#include "GameLoop.h"
#include "SceneTree.h"
#include "Nodes/NodeWindow.h"

#include "Scripting/Lua/LuaScriptInstance.hpp"
#include "Scripting/Proxies/ActionMapProxy.hpp"

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Root");
    uptr<Node> scene = Node::CreateNode<Node>("Scene");    //Load this with the default .st file

    uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>("res/test.lua");
    Node::AttachScript(script, *scene);
    root->AddChild(scene);

    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.StartGame(defaultSceneTree);
}

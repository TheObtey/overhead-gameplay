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
    uptr<Node> scene = Node::LoadNodeFromJSON<Node>("res/scenes/default.sc.json");
    uptr<NodeViewport> viewport = Node::CreateNode<NodeViewport>("MainViewport");
    viewport->AddChild(std::move(scene));
    return viewport;
}

int main()
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Window");
    SceneTree defaultSceneTree(root);

    GameLoop loop;
    loop.LoadScene = LoadScene;
    loop.StartGame(defaultSceneTree);
}

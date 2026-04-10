#include "Scripting/Proxies/SceneTreeProxy.h"
#include "Registries/AutomaticRegisterProxy.hpp"
#include "Servers/EngineServer.h"
#include "SceneTree.h"

SceneTreeProxy::SceneTreeProxy(SceneTree *tree) : m_sceneTree(tree)
{
    tree->OnGameStarted  += [&]()   { if (OnGameStarted)  OnGameStarted(); };
    tree->OnGameEnded    += [&]()   { if (OnGameEnded)    OnGameEnded(); };
    tree->OnSceneChanged += [&]()   { if (OnSceneChanged) OnSceneChanged(); };
}

void SceneTreeProxy::ChangeSceneToNode(NodeProxy const& node)
{
    uptr<Node> unode;
    if (node.HasParent())
        unode = std::move(EngineServer::GetUnattachedNode(node.GetProxyOwner()));
    else
        unode = node.GetProxyOwner()->DetachFromTree();

    m_sceneTree->ChangeSceneToNode(unode);
}

SceneTreeProxy::NodeProxy& SceneTreeProxy::GetCurrentScene() const
{
    return m_sceneTree->GetCurrentScene().GetNodeProxy();
}

SceneTreeProxy::NodeProxy& SceneTreeProxy::GetRoot() const
{
    return m_sceneTree->GetRoot().GetNodeProxy();
}

void SceneTreeProxyBinding::Bind(Binder &binder)
{
    binder.BindClass<SceneTreeProxy>("scenetree",
        "ChangeSceneToNode", &SceneTreeProxy::ChangeSceneToNode,
        "GetCurrentScene", &SceneTreeProxy::GetCurrentScene,
        "GetRoot", &SceneTreeProxy::GetRoot,
        "OnGameStarted", &SceneTreeProxy::OnGameStarted,
        "OnGameEnded", &SceneTreeProxy::OnGameEnded,
        "OnSceneChanged", &SceneTreeProxy::OnSceneChanged
    );
}

REGISTER_PROXY(SceneTreeProxyBinding, SceneTreeProxy_);
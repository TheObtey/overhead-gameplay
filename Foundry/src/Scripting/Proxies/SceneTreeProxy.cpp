#include "Scripting/Proxies/SceneTreeProxy.h"
#include "SceneTree.h"
#include "Servers/EngineServer.h"

SceneTreeProxy::SceneTreeProxy(SceneTree *tree) : m_sceneTree(tree)
{
    tree->OnGameStarted += [&]()    { OnGameStarted(); };
    tree->OnGameEnded += [&]()      { OnGameEnded(); };
    tree->OnSceneChanged += [&]()   { OnSceneChanged(); };
}

void SceneTreeProxy::ChangeSceneToNode(NodeProxy const& node)
{
    uptr<Node> unode = node.HasParent() ?
        std::move(EngineServer::GetUnattachedNode(node.GetProxyOwner())) :
        node.GetProxyOwner()->DetachFromTree();
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

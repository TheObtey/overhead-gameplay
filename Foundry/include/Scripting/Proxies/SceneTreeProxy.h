#ifndef FOUNDRY_SCENETREEPROXY__H_
#define FOUNDRY_SCENETREEPROXY__H_

#include "Node.h"

class Binder;
class SceneTree;
class SceneTreeProxy
{
using NodeProxy = Node::Proxy;

public:
    explicit SceneTreeProxy(SceneTree* tree);
    ~SceneTreeProxy() = default;

    void ChangeSceneToNode(const NodeProxy& node);
    NodeProxy& GetCurrentScene() const;
    NodeProxy& GetRoot() const;

    std::function<void()> OnGameStarted;
    std::function<void()> OnGameEnded;
    std::function<void()> OnSceneChanged;

private:
    SceneTree* m_sceneTree;
};

struct SceneTreeProxyBinding
{
    static void Bind(Binder& binder);
};


#endif
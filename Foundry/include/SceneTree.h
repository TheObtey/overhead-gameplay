#ifndef FOUNDRY_SCENE_TREE__H_
#define FOUNDRY_SCENE_TREE__H_

#include "Node.h"

class SceneTree
{
public:
    SceneTree() = default;
    SceneTree(uptr<Node>& rootNode);
    SceneTree(SceneTree const& other);
    SceneTree(SceneTree&& other) noexcept = delete;

    SceneTree& operator=(SceneTree const& other);
    SceneTree& operator=(SceneTree&& other) noexcept = delete;

    ~SceneTree() = default;

    void ChangeSceneToNode(Node& newScene);

    Node& GetCurrentScene() const { return *m_pCurrentScene; }
    Node& GetRoot() const { return *m_root; }

    Event<void()> OnGameStarted;
    Event<void()> OnGameEnded;
    Event<void(Node& node)> OnNodeAdded;
    Event<void(Node& node)> OnNodeRemoved;
    Event<void()> OnSceneChanged;

private:
    //Where the scene begin, a direct child of the root
    Node* m_pCurrentScene = nullptr;
    uptr<Node> m_root;

    friend class Node;
};

#endif

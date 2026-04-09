#include "SceneTree.h"

SceneTree::SceneTree(uptr<Node>& rootNode)
{
    rootNode->m_pSceneTree = this;
    m_root = std::move(rootNode);
}

SceneTree::SceneTree(SceneTree const& other)
{
    m_root = other.m_root->Clone();
}

SceneTree& SceneTree::operator=(SceneTree const& other)
{
    if (this == &other) return *this;
    m_root = other.m_root->Clone();
    return *this;
}

void SceneTree::ChangeSceneToNode(uptr<Node>& newScene)
{
    newScene->m_pSceneTree = this;
    if (m_pCurrentScene) m_pCurrentScene->Destroy();
    m_pCurrentScene = newScene.get();
    m_root->GetChild(0).AddChild(std::move(newScene));

    OnSceneChanged();
}

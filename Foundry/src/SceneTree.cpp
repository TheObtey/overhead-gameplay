#include "SceneTree.h"

SceneTree::SceneTree(uptr<Node>& rootNode)
{
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

void SceneTree::ChangeSceneToNode(Node& newScene)
{
    if (m_pCurrentScene) m_pCurrentScene->Destroy();

}

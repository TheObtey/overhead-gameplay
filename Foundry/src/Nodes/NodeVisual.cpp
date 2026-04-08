#include "Nodes/NodeVisual.h"
#include "Nodes/NodeViewport.h"

NodeVisual::NodeVisual(::std::string const &name) : Node3D(name)
{
    OnHierarchyChanged += [&](){ TryAttachToViewport(); };
}

ISerializable * NodeVisual::CreateInstance()
{
    return Node::CreateNode<NodeVisual>("NodeVisual").release();
}

bool NodeVisual::IsVisible()
{
    //TODO CAMERA CULLING AND OTHER
    return true;
}

void NodeVisual::TryAttachToViewport()
{
    if (const auto viewport = FindFirstParentOfType<NodeViewport>())
        m_pViewport = &viewport->get();
}

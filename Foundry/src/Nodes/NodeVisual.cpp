#include "Nodes/NodeVisual.h"
#include "Nodes/NodeViewport.h"

NodeVisual::NodeVisual(::std::string const &name) : Node3D(name)
{
    OnParentChange += [&](Node& node)
    {
        if (const auto viewport = FindFirstParentOfType<NodeViewport>())
            m_pViewport = &viewport->get();
    };
}

void NodeVisual::OnUpdate(double delta)
{
    Node3D::OnUpdate(delta);
}

ISerializable * NodeVisual::CreateInstance()
{
    return Node::CreateNode<NodeVisual>("NodeVisual").release();
}

bool NodeVisual::Visible()
{
    //TODO CAMERA CULLING AND OTHER
    return true;
}

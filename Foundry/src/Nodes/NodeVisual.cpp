#include "Nodes/NodeVisual.h"
#include "Nodes/NodeViewport.h"

#include "Serialization/SerializeObject.hpp"
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

void NodeVisual::Serialize(SerializedObject &datas) const
{
    Node3D::Serialize(datas);
    datas.SetType("NodeVisual");
}

void NodeVisual::Deserialize(SerializedObject const &datas)
{
    Node3D::Deserialize(datas);
}

uptr<Node> NodeVisual::Clone()
{
	uptr<NodeVisual> clone = Node::CreateNode<NodeVisual>(GetName());

	SerializedObject datas;
	Serialize(datas);
	clone->Deserialize(datas);

	return clone;
}

void NodeVisual::AttachScriptDeserialize(uptr<LuaScriptInstance>& script)
{
    AttachScript<NodeVisual>(script, *this);
}

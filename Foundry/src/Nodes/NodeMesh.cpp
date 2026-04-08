#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "GeometryFactory.h"
#include "Servers/GraphicServer.h"

NodeMesh::NodeMesh(std::string const& name) : NodeVisual(name)
{
    m_pMesh = std::make_unique<Ore::Mesh>();
    m_pMesh->SetGeometry(GraphicServer::GetDefaultGeo());
    //problem here
    AddTextures(GraphicServer::GetDefaultTexture());
    m_pMesh->SetTransform(m_transform.GetMatrix());
}

void NodeMesh::OnUpdate(double delta)
{
    NodeVisual::OnUpdate(delta);
    m_pMesh->SetTransform(m_transform.GetMatrix());

    if (IsVisible() && m_pViewport)
        m_pViewport->AddMesh(*this);
}

bool NodeMesh::IsVisible()
{
    //TODO CULLING
    return true;
}

void NodeMesh::SetGeometry(sptr<Ore::Geometry> const& geometry) const
{
    m_pMesh->SetGeometry(geometry);
}

ISerializable* NodeMesh::CreateInstance()
{
    return Node::CreateNode<NodeMesh>("NodeMesh").release();
}

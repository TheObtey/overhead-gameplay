#include "Nodes/NodeCamera.h"

#include "MathUtils.h"
#include "Nodes/NodeViewport.h"
#include "Servers/GraphicServer.h"

NodeCamera::NodeCamera(std::string const &name) : Node3D(name)
{
    SetFOV(45);
    m_camera.Perspective.up = {0.0f, 1.0f, 0.0f};
    OnHierarchyChanged += [&](){ TryAttachToViewport(); };
}

void NodeCamera::OnUpdate(double delta)
{
    bool need_update = IsTransformDirty();
    Node3D::OnUpdate(delta);

    if (need_update)
    {
        m_camera.SetTransform(m_transform.GetMatrix());
        m_camera.UpdateCamera();
    }
}

void NodeCamera::Serialize(SerializedObject &datas) const
{
    Node3D::Serialize(datas);
}

void NodeCamera::Deserialize(SerializedObject const &datas)
{
    Node3D::Deserialize(datas);
}

ISerializable* NodeCamera::CreateInstance()
{
    return CreateNode<NodeCamera>("NodeCamera").release();
}

void NodeCamera::TryAttachToViewport()
{
    if ( const auto viewport = FindFirstParentOfType<NodeViewport>())
        GraphicServer::AttachToViewport(this, &viewport->get());
}

void NodeCamera::UpdateCameraOwner(NodeViewport& newOwner)
{
    if (m_pCurrentViewport) m_pCurrentViewport->SetCamera(nullptr);
    m_pCurrentViewport = &newOwner;
    m_pCurrentViewport->OnViewportResize += [&](uint32 const width, uint32 const height)
    {
        m_camera.Perspective.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        m_camera.UpdateCamera();
    };
    newOwner.SetCamera(this);
}

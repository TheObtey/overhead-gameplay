#include "Nodes/NodeCamera.h"

void NodeCamera::OnUpdate(double delta)
{
    bool need_update = IsTransformDirty();
    Node3D::OnUpdate(delta);

    if (need_update)
    {
        m_camera.SetYaw(m_transform.GetYaw());
        m_camera.SetPitch(m_transform.GetPitch());
        m_camera.SetRoll(m_transform.GetRoll());
        m_camera.SetPosition(m_transform.GetPosition());
        m_camera.UpdateVectors();
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

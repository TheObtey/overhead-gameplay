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
        m_camera.SetTransform(m_worldTransform);
        m_camera.UpdateCamera();
    }
}

void NodeCamera::Serialize(SerializedObject& datas) const
{
    Node3D::Serialize(datas);
    datas.SetType("NodeCamera");

    float fov = m_camera.Perspective.fov * (180/pi_t<long double>);
    float nearPlane = m_camera.Perspective.nearPlane;
    float farPlane = m_camera.Perspective.farPlane;
    float aspectRatio = m_camera.Perspective.aspectRatio;
    glm::vec3 up = m_camera.Perspective.up;
    datas.AddPublicElement("FOV", &fov);
    datas.AddPublicElement("NearPlane", &nearPlane);
    datas.AddPublicElement("FarPlane", &farPlane);
    datas.AddPrivateElement("AspectRatio", &aspectRatio);
    datas.AddPublicElement("Up", &up);
}

void NodeCamera::Deserialize(SerializedObject const& datas)
{
    Node3D::Deserialize(datas);

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float aspectRatio = 1920.0f/1080.0f;
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };

    datas.GetPublicElement("FOV", &fov);
    datas.GetPublicElement("NearPlane", &nearPlane);
    datas.GetPublicElement("FarPlane", &farPlane);
    if (!datas.TryGetPrivateElement("AspectRatio", &aspectRatio)) {
        datas.TryGetPublicElement("AspectRatio", &aspectRatio);
    }
    datas.GetPublicElement("Up", &up);

    m_camera.Perspective.fov = fov * (pi_t<long double> / 180);
    m_camera.Perspective.nearPlane = nearPlane;
    m_camera.Perspective.farPlane = farPlane;
    m_camera.Perspective.aspectRatio = aspectRatio;
    m_camera.Perspective.up = up;

    m_camera.SetTransform(m_worldTransform);
    m_camera.UpdateCamera();
}

void NodeCamera::AttachScriptDeserialize(uptr<LuaScriptInstance>& script)
{
    AttachScript<NodeCamera>(script, *this);
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

uptr<Node> NodeCamera::Clone()
{
	uptr<NodeCamera> clone = Node::CreateNode<NodeCamera>(GetName());

	SerializedObject datas;
	Serialize(datas);
	clone->Deserialize(datas);

	return clone;
}

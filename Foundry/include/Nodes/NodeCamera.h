#ifndef FOUNDRY_NODECAMERA__H_
#define FOUNDRY_NODECAMERA__H_

#include "Camera.h"
#include "Node3D.h"

class NodeViewport;

class NodeCamera : public Node3D
{
public:
    class Proxy;

    NodeCamera(std::string const& name);
    ~NodeCamera() override = default;

    void SetFOV(float const fov) { m_camera.Perspective.fov = fov; }

    virtual void OnUpdate(double delta) override;
    virtual void Reparent(Node& newParent, bool keepGlobalTransform = true) override {};
    virtual void Serialize(SerializedObject& datas) const override;
    virtual void Deserialize(SerializedObject const& datas) override;

    static ISerializable* CreateInstance();

private:
    void TryAttachToViewport();
    void UpdateCameraOwner(NodeViewport& newOwner);

protected:
    Ore::Camera m_camera;
    NodeViewport* m_pCurrentViewport = nullptr;

    friend class GraphicServer;
    friend class NodeViewport;
};

REGISTER_ISERIALIZABLE(NodeCamera, NodeCamera::CreateInstance);

#include "Scripting/Proxies/NodeCameraProxy.inl"

#endif

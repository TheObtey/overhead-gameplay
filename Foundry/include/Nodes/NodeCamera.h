#ifndef FOUNDRY_NODECAMERA__H_
#define FOUNDRY_NODECAMERA__H_

#include "Camera.h"
#include "Node3D.h"

class NodeCamera : public Node3D
{
public:
    class Proxy;

    NodeCamera(std::string const& name) : Node3D(name) {};
    ~NodeCamera() override = default;

    void SetFOV(float const fov) { m_camera.SetFov(fov); }

    virtual void OnUpdate(double delta) override;
    virtual void Reparent(Node& newParent, bool keepGlobalTransform = true) override {};
    virtual void Serialize(SerializedObject& datas) const override;
    virtual void Deserialize(SerializedObject const& datas) override;

    static ISerializable* CreateInstance();

protected:
    Camera m_camera;
};

REGISTER_ISERIALIZABLE(NodeCamera, NodeCamera::CreateInstance);

#include "Scripting/Proxies/NodeCameraProxy.inl"

#endif
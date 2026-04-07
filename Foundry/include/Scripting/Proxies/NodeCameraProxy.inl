class NodeCamera::Proxy : public Node3D::Proxy
{
public:
    struct ProxyBinding;

    Proxy(Node& node) : Node3D::Proxy(node), m_pNodeCamera(static_cast<NodeCamera*>(&node)) {}
    void SetFOV(float fov) { m_pNodeCamera->SetFOV(fov); }

private:
    NodeCamera* m_pNodeCamera;
};

BindProxy(NodeCamera::Proxy,
    return binder.BindClass<NodeCamera::Proxy>("nodecamera",
    sol::base_classes, sol::bases<Node3D::Proxy>(),
    "SetFOV", BIND(SetFOV));
)

REGISTER_PROXY(NodeCamera::Proxy::ProxyBinding, NodeCameraProxy);
